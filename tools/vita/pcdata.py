#!/usr/bin/env python3
# SPDX-License-Identifier: AGPL-3.0-or-later
"""Build-time generator for the PC-only data files of the PC/Vita port.

Some overlays still keep part of their data as raw assembly on PSX, so the PC
port compiles a C copy of it. Those copies contain bytes taken from the game
disc and are not committed. Instead, a template is committed for each of them
under tools/vita/pcdata/: the template is the C file where every run of
numeric initializer lines is replaced by a placeholder that records where the
values live in the user's own disc files and how they were formatted.

    pcdata.py generate TEMPLATE OUTPUT
        Expand a template into the C file, reading the disc files.

    pcdata.py generate-all tools/vita/pcdata
        Expand every TEMPLATE.in of the tree into the same path relative to
        the repository root (see tools/vita/generate_pc_data.sh).

    pcdata.py template --source FILE [--source FILE ...] INPUT TEMPLATE
        Create a template from an existing C file (maintainer helper). Every
        run of numeric lines is looked up in the given disc files; runs that
        cannot be found are kept as they are and reported.

Placeholder syntax (one line each):
    @@PCDATA-SOURCE <id> <path relative to the repository root> <sha1>
    @@PCDATA <id|zero> <offset> <fmt> <indent> <rows> <last-comma> [patch]
fmt is x2 (u8 as 0x%02X), x4 (u16 as 0x%04X) or x8 (u32 as 0x%08X), values
are little endian. rows is the number of values on each line, with NxM
meaning M lines of N values. The optional patch is a list of pos:byte that
overrides a few bytes of the run (pointers the PSX data relocates).
"""
import argparse
import hashlib
import os
import re
import sys

FORMATS = {
    "x2": (1, re.compile(r"0x[0-9A-F]{2}$"), "0x%02X"),
    "x4": (2, re.compile(r"0x[0-9A-F]{4}$"), "0x%04X"),
    "x8": (4, re.compile(r"0x[0-9A-F]{8}$"), "0x%08X"),
}
LITERAL_LINE = re.compile(r"^( +)(0x[0-9A-F]+(?:, 0x[0-9A-F]+)*)(,?)$")


def read_text(path):
    with open(path, "r", encoding="utf-8", newline="") as f:
        return f.read()


def write_text(path, text):
    os.makedirs(os.path.dirname(path) or ".", exist_ok=True)
    with open(path, "w", encoding="utf-8", newline="") as f:
        f.write(text)


def encode_rows(rows):
    out = []
    i = 0
    while i < len(rows):
        j = i
        while j < len(rows) and rows[j] == rows[i]:
            j += 1
        out.append("%dx%d" % (rows[i], j - i) if j - i > 1 else str(rows[i]))
        i = j
    return ",".join(out)


def decode_rows(spec):
    rows = []
    for part in spec.split(","):
        if "x" in part:
            n, m = part.split("x")
            rows.extend([int(n)] * int(m))
        else:
            rows.append(int(part))
    return rows


def parse_line(line):
    """Return (indent, fmt, values, has_comma) for a pure numeric line."""
    m = LITERAL_LINE.match(line)
    if not m:
        return None
    tokens = m.group(2).split(", ")
    for fmt, (size, regex, _) in FORMATS.items():
        if all(regex.match(t) for t in tokens):
            return len(m.group(1)), fmt, [int(t, 16) for t in tokens], m.group(3) == ","
    return None


def to_bytes(fmt, values):
    size = FORMATS[fmt][0]
    return b"".join(v.to_bytes(size, "little") for v in values)


def expand(fmt, data, indent, rows, last_comma):
    size, _, pattern = FORMATS[fmt]
    values = [int.from_bytes(data[i:i + size], "little")
              for i in range(0, len(data), size)]
    lines = []
    pos = 0
    for i, n in enumerate(rows):
        text = " " * indent + ", ".join(pattern % v for v in values[pos:pos + n])
        if i < len(rows) - 1 or last_comma:
            text += ","
        lines.append(text)
        pos += n
    return lines


def cmd_generate(template, output, root):
    sources = {}
    blobs = {}
    out = []
    for line in read_text(template).split("\n"):
        if line.startswith("@@PCDATA-SOURCE "):
            _, sid, path, sha1 = line.split(" ")
            sources[sid] = (path, sha1)
            continue
        if line.startswith("@@PCDATA "):
            fields = line.split(" ")
            _, sid, off, fmt, indent, rows, last_comma = fields[:7]
            patch = fields[7] if len(fields) > 7 else ""
            rows = decode_rows(rows)
            length = sum(rows) * FORMATS[fmt][0]
            if sid == "zero":
                data = bytes(length)
            else:
                if sid not in blobs:
                    path = os.path.join(root, sources[sid][0])
                    if not os.path.exists(path):
                        sys.exit("pcdata: missing %s (extract the disc first)" % path)
                    with open(path, "rb") as f:
                        blobs[sid] = f.read()
                    # offsets are only valid for the US release
                    if hashlib.sha1(blobs[sid]).hexdigest() != sources[sid][1]:
                        sys.exit("pcdata: %s does not match the US release" % path)
                off = int(off, 16)
                data = blobs[sid][off:off + length]
                if len(data) != length:
                    sys.exit("pcdata: %s is too short for %s" % (sources[sid][0], template))
            if patch:
                data = bytearray(data)
                for item in patch.split(","):
                    pos, value = item.split(":")
                    data[int(pos, 16)] = int(value, 16)
            out.extend(expand(fmt, data, int(indent), rows, last_comma == "1"))
            continue
        out.append(line)
    text = "\n".join(out)
    # leave up to date files untouched so the build does not redo them
    if os.path.exists(output) and read_text(output) == text:
        return False
    write_text(output, text)
    return True


def cmd_generate_all(directory, root):
    count = changed = 0
    for base, dirs, files in os.walk(directory):
        dirs.sort()
        for name in sorted(files):
            if not name.endswith(".in"):
                continue
            template = os.path.join(base, name)
            output = os.path.join(root, os.path.relpath(template, directory)[:-3])
            count += 1
            if cmd_generate(template, output, root):
                changed += 1
                print("pcdata: wrote %s" % os.path.relpath(output, root))
    print("pcdata: %d files, %d updated" % (count, changed))


def cmd_template(sources, input_path, template, root):
    blobs = []
    for path in sources:
        with open(os.path.join(root, path), "rb") as f:
            blobs.append(f.read())
    lines = read_text(input_path).split("\n")
    out = ["@@PCDATA-SOURCE %d %s %s" % (i, p, hashlib.sha1(b).hexdigest())
           for i, (p, b) in enumerate(zip(sources, blobs))]
    hint = [0] * len(blobs)
    kept = 0

    def find(data):
        # prefer the occurrence right after the previous match
        for i, blob in enumerate(blobs):
            pos = blob.find(data, hint[i])
            if pos < 0:
                pos = blob.find(data)
            if pos >= 0:
                return i, pos
        return None

    i = 0
    while i < len(lines):
        first = parse_line(lines[i])
        if not first:
            out.append(lines[i])
            i += 1
            continue
        indent, fmt = first[0], first[1]
        # collect the run: same indent and format, only the last may lack ','
        run = [first]
        j = i + 1
        while run[-1][3] and j < len(lines):
            p = parse_line(lines[j])
            if not p or p[0] != indent or p[1] != fmt:
                break
            run.append(p)
            j += 1
        # greedily emit the longest prefix found in the sources
        k = 0
        while k < len(run):
            lo, hi, best = k + 1, len(run), None
            while lo <= hi:
                mid = (lo + hi) // 2
                data = b"".join(to_bytes(fmt, r[2]) for r in run[k:mid])
                hit = find(data)
                if hit:
                    best = (mid, hit, data)
                    lo = mid + 1
                else:
                    hi = mid - 1
            if best is None:
                # all-zero lines come from .bss or padding
                e = k
                while e < len(run) and not any(run[e][2]):
                    e += 1
                if e > k:
                    best = (e, None, None)
            patch = ""
            if best is None and blobs:
                # a line that differs from the disc in a few bytes (e.g. a
                # pointer cleared by the original generator): take it from
                # where the previous run ended and record the differences
                data = to_bytes(fmt, run[k][2])
                cands = []
                for pos in range(max(0, hint[0] - 32), hint[0] + 33):
                    ref = blobs[0][pos:pos + len(data)]
                    if len(ref) == len(data):
                        diff = [(n, b) for n, b in enumerate(data) if ref[n] != b]
                        cands.append((len(diff), abs(pos - hint[0]), pos, diff))
                if cands:
                    _, _, pos, diff = min(cands)
                    if len(diff) <= len(data) // 2:
                        best = (k + 1, (0, pos), data)
                        patch = " " + ",".join("%X:%02X" % d for d in diff)
            if best is None:
                out.append(lines[i + k])
                print("%s:%d: kept as is" % (input_path, i + k + 1), file=sys.stderr)
                kept += 1
                k += 1
                continue
            end, hit, data = best
            rows = [len(r[2]) for r in run[k:end]]
            last_comma = "1" if run[end - 1][3] else "0"
            if hit is None:
                sid, off = "zero", "0"
            else:
                sid, off = str(hit[0]), "%X" % hit[1]
                hint[hit[0]] = hit[1] + len(data)
            out.append("@@PCDATA %s %s %s %d %s %s%s" %
                       (sid, off, fmt, indent, encode_rows(rows), last_comma, patch))
            k = end
        i = j
    write_text(template, "\n".join(out))
    return kept


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--root", default=".", help="repository root")
    sub = ap.add_subparsers(dest="cmd", required=True)
    g = sub.add_parser("generate")
    g.add_argument("template")
    g.add_argument("output")
    a = sub.add_parser("generate-all")
    a.add_argument("directory", help="template tree mirroring the repository")
    t = sub.add_parser("template")
    t.add_argument("--source", action="append", required=True)
    t.add_argument("input")
    t.add_argument("template")
    args = ap.parse_args()
    if args.cmd == "generate":
        cmd_generate(args.template, args.output, args.root)
    elif args.cmd == "generate-all":
        cmd_generate_all(args.directory, args.root)
    else:
        kept = cmd_template(args.source, args.input, args.template, args.root)
        if kept:
            print("%s: %d numeric lines kept" % (args.input, kept), file=sys.stderr)


if __name__ == "__main__":
    main()
