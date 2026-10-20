#!/usr/bin/env python3
# SPDX-License-Identifier: AGPL-3.0-or-later
"""Strips the bytes taken from the game disc out of a linked PS Vita ELF.

The Vita executable compiles data tables that come from the game disc: the
sotn-assets headers (src/**/gen/), the PC data files (tools/vita/pcdata.py)
and the data of the decompiled sources. A release build must not contain
them, so this tool finds every run of the loaded data of the linked ELF that
appears verbatim in the disc files, zeroes it and writes a manifest telling
the game where to read it back from at startup (vita_assets.c). The
manifest only holds offsets, lengths and checksums.

    mkblank.py blank --elf build/sotn --disc disks/us --map build/sotn.map \
        --out-elf build/sotn_release --manifest build/sotn_assets.bin \
        [--report build/mkblank_report.txt] [--dry-run [--explain SYM]]
        Find the disc bytes, write the blanked ELF and the manifest.

    mkblank.py verify --elf build/sotn --blank-elf build/sotn_release \
        --manifest build/sotn_assets.bin --disc disks/us
        Restore the blanked ELF from the disc like the game does and check
        that every loaded byte equals the original ELF.

    mkblank.py scan --disc disks/us [--elf ELF --map MAP] [--strict] FILE...
        Report the runs of the given ELF, velf or eboot.bin (SELF) files
        found on the disc. Loaded segments are scanned, compressed SELF
        segments are decompressed first.

    mkblank.py check-dump --elf build/sotn --manifest build/sotn_assets.bin \
        DUMP
        Compare the data dumped by the game after the restore (see
        vita_assets.c) with the original ELF.

How it works: every 16-byte window of the data is hashed, the disc files are
hashed at every offset and looked up, and each hit is extended forwards and
backwards byte by byte, trying the first and the last disc location of the
window and the continuation of the previous match. Pointers (relocated
words) differ between the PSX and the Vita: they are never blanked, and a
match runs through them where the disc holds a PSX RAM address, so tables of
structs holding pointers are covered too. Windows made of a repeated 1, 2 or
4-byte unit do not start a match; the stretches left between matches are
then looked up on the disc as a whole. Data of the toolchain and of the Vita
backend, which run before the restore, is never blanked (link map).
"""
import argparse
import hashlib
import os
import re
import struct
import sys
import zlib

import numpy as np

K = 16  # window size, also the minimum length of a match

MANIFEST_MAGIC = b"SOTNRST1"
MANIFEST_VERSION = 1
BUILD_ID_SYMBOL = "g_VitaAssetsBuildId"
BUILD_ID_MAGIC = b"SOTN-ASSETS-ID\x00\x00"

# Disc files that are never compiled into the executable: movies, streamed
# audio and the raw CD-DA track
DISC_SKIP = re.compile(r"(\.STR$|^XA/|^SD/XA_STR|\.DA$)", re.IGNORECASE)

# Objects of the link map whose data must stay in the executable, as they run
# before the restore: the toolchain (absolute paths, the project's objects
# are relative to the build directory) and the Vita backend of psyz, which
# includes the restore itself (vita_assets.c)
DEFAULT_KEEP = [r"^/", r"libpsyz\.a\(vita_\w+\.c\.obj\)$",
                r"/src/vita/vita_\w+\.c\.obj$"]

SHF_WRITE, SHF_ALLOC, SHF_EXECINSTR = 1, 2, 4
SHT_PROGBITS, SHT_SYMTAB, SHT_NOBITS, SHT_REL = 1, 2, 8, 9
PT_LOAD = 1
# 32-bit data relocations
R_DATA32 = {2, 3, 38, 41, 42}  # ABS32, REL32, TARGET1, PREL31, TARGET2


# ---------------------------------------------------------------- ELF ------


class Section:
    def __init__(self, idx, name, sh):
        self.idx = idx
        self.name = name
        (_, self.type, self.flags, self.addr, self.offset, self.size,
         self.link, self.info, self.align, self.entsize) = sh

    @property
    def loaded(self):
        return self.flags & SHF_ALLOC and self.type != SHT_NOBITS and self.size


class Elf:
    def __init__(self, path):
        self.path = path
        with open(path, "rb") as f:
            self.raw = bytearray(f.read())
        raw = self.raw
        if raw[:4] != b"\x7fELF" or raw[4] != 1 or raw[5] != 1:
            sys.exit(f"{path}: not a 32-bit little endian ELF")
        (self.e_type, _, _, self.entry, self.phoff, self.shoff, _, _,
         self.phentsize, self.phnum, self.shentsize, self.shnum,
         self.shstrndx) = struct.unpack_from("<HHIIIIIHHHHHH", raw, 16)
        self.phdrs = [struct.unpack_from("<8I", raw, self.phoff + i * 32)
                      for i in range(self.phnum)]
        shs = [struct.unpack_from("<10I", raw, self.shoff + i * 40)
               for i in range(self.shnum)]
        strtab = shs[self.shstrndx]
        self.sections = []
        for i, sh in enumerate(shs):
            self.sections.append(Section(i, self._str(strtab[4], sh[0]), sh))
        self._symbols = None

    def _str(self, off, idx):
        end = self.raw.index(b"\0", off + idx)
        return self.raw[off + idx:end].decode("latin-1")

    def section_bytes(self, sec):
        return np.frombuffer(bytes(self.raw[sec.offset:sec.offset + sec.size]),
                             dtype=np.uint8)

    def symbols(self):
        """[(name, value, size, type, shndx)] of the static symbol table"""
        if self._symbols is None:
            self._symbols = []
            for sec in self.sections:
                if sec.type != SHT_SYMTAB:
                    continue
                stroff = self.sections[sec.link].offset
                for i in range(sec.size // 16):
                    name, value, size, info, _, shndx = struct.unpack_from(
                        "<IIIBBH", self.raw, sec.offset + i * 16)
                    self._symbols.append((self._str(stroff, name), value,
                                          size, info & 15, shndx))
        return self._symbols

    def symbol(self, name):
        found = [s for s in self.symbols() if s[0] == name]
        if len(found) != 1:
            sys.exit(f"{self.path}: expected one symbol {name}, "
                     f"found {len(found)}")
        return found[0]

    def reloc_slots(self, sec):
        """addresses of the 32-bit words of `sec` that get relocated"""
        out = []
        for rs in self.sections:
            if rs.type != SHT_REL or rs.info != sec.idx:
                continue
            ents = np.frombuffer(bytes(self.raw[rs.offset:rs.offset + rs.size]),
                                 dtype="<u4").reshape(-1, 2)
            types = ents[:, 1] & 0xFF
            other = set(np.unique(types).tolist()) - R_DATA32
            if other:
                sys.exit(f"{self.path}: unexpected relocation types {other} "
                         f"in {sec.name}")
            out.append(ents[:, 0])
        return np.concatenate(out) if out else np.zeros(0, np.uint32)

    def load_segments(self):
        """[(vaddr, file bytes)] of the PT_LOAD segments"""
        out = []
        for ph in self.phdrs:
            if ph[0] == PT_LOAD and ph[4]:
                out.append((ph[2], bytes(self.raw[ph[1]:ph[1] + ph[4]])))
        return out

    def vaddr_to_offset(self, addr, size):
        for sec in self.sections:
            if sec.loaded and sec.addr <= addr and addr + size <= sec.addr + sec.size:
                return sec.offset + addr - sec.addr
        raise ValueError(f"{addr:#x} is not in a loaded section")


def data_sections(elf):
    """loaded sections that hold data (not code)"""
    return [s for s in elf.sections
            if s.loaded and not s.flags & SHF_EXECINSTR
            and s.type not in (SHT_REL,) and s.name not in (".ARM.exidx",)]


def segment_of(elf, addr):
    for i, ph in enumerate([p for p in elf.phdrs if p[0] == PT_LOAD]):
        if ph[2] <= addr < ph[2] + ph[5]:
            return i
    raise ValueError(f"{addr:#x} is not in a PT_LOAD segment")


def build_id(elf):
    h = hashlib.sha1()
    for vaddr, data in elf.load_segments():
        h.update(struct.pack("<I", vaddr))
        h.update(data)
    return h.digest()[:16]


# ---------------------------------------------------------- link map ------


def parse_map(path):
    """[(addr, size, object, merged, section)] of every input section of a
    GNU ld map. `merged` is set for the sections whose constants the linker
    merged into a pool shared by all objects (listed under the first one)."""
    out = []
    pending = None
    line_re = re.compile(
        r"^ (\S+)?\s+0x([0-9a-fA-F]+)\s+0x([0-9a-fA-F]+)\s+(\S.*)$")
    in_map = False
    with open(path, "r", errors="replace") as f:
        for line in f:
            line = line.rstrip("\n")
            if line.startswith("Linker script and memory map"):
                in_map = True
                continue
            if not in_map:
                continue
            if line.endswith("(size before relaxing)") and out:
                out[-1] = out[-1][:3] + (True, out[-1][4])
                continue
            m = line_re.match(line)
            if m:
                name = m.group(1) or pending
                pending = None
                if name and not name.startswith("*"):
                    out.append((int(m.group(2), 16), int(m.group(3), 16),
                                m.group(4).strip(), False, name))
                continue
            m = re.match(r"^ (\.\S+|[A-Za-z_]\w*)$", line)
            pending = m.group(1) if m else None
    return out


# ----------------------------------------------------------- hashing ------

C1 = np.uint64(0x9E3779B97F4A7C15)
C2 = np.uint64(0xC2B2AE3D27D4EB4F)


def window_hashes(a):
    """64-bit hash of every K-byte window of the uint8 array `a`"""
    n = len(a) - K + 1
    if n <= 0:
        return np.zeros(0, np.uint64)
    w = [np.zeros(n, np.uint64), np.zeros(n, np.uint64)]
    a64 = a.astype(np.uint64)
    for half in range(2):
        for j in range(8):
            w[half] |= a64[half * 8 + j:half * 8 + j + n] << np.uint64(8 * j)
    with np.errstate(over="ignore"):
        h = w[0] * C1
        h ^= (w[1] * C2) >> np.uint64(29)
        h ^= w[1]
    return h


def low_entropy(a):
    """True for each K-byte window made of a repeating 1, 2 or 4 byte unit"""
    n = len(a) - K + 1
    out = np.zeros(max(n, 0), bool)
    if n <= 0:
        return out
    for d in (1, 2, 4):
        eq = np.concatenate([[0], np.cumsum(a[:-d] == a[d:], dtype=np.int64)])
        cnt = eq[K - d:K - d + n] - eq[:n]
        out |= cnt == K - d
    return out


def window_ok(mask):
    """True for each K-byte window whose bytes are all set in `mask`"""
    n = len(mask) - K + 1
    if n <= 0:
        return np.zeros(0, bool)
    c = np.concatenate([[0], np.cumsum(mask, dtype=np.int64)])
    return (c[K:K + n] - c[:n]) == K


def psx_pointer_mask(d):
    """True on the bytes of the aligned words of `d` that look like PSX RAM
    addresses, the words the Vita relocates"""
    n4 = len(d) // 4 * 4
    w = d[:n4].view("<u4")
    m = (w >= 0x80000000) & (w < 0x80200000)
    out = np.zeros(len(d), bool)
    out[:n4] = np.repeat(m, 4)
    return out


class Disc:
    def __init__(self, root):
        self.root = root
        self.names = []
        for dirpath, dirs, files in os.walk(root):
            dirs.sort()
            for fn in sorted(files):
                rel = os.path.relpath(os.path.join(dirpath, fn), root)
                rel = rel.replace(os.sep, "/")
                if not DISC_SKIP.search(rel):
                    self.names.append(rel)
        if not self.names:
            sys.exit(f"{root}: no disc files")
        self._data = {}
        self._ptr = {}

    def data(self, idx):
        if idx not in self._data:
            with open(os.path.join(self.root, self.names[idx]), "rb") as f:
                self._data[idx] = np.frombuffer(f.read(), dtype=np.uint8)
        return self._data[idx]

    def pointers(self, idx):
        if idx not in self._ptr:
            self._ptr[idx] = psx_pointer_mask(self.data(idx))
        return self._ptr[idx]

    def lookup(self, hashes, masked):
        """first and last (file, offset) of each hash, file -1 if absent.
        The hashes flagged in `masked` are looked up in the disc with its
        pointers zeroed, the others in the raw disc."""
        out = [np.full(len(hashes), -1, np.int64) for _ in range(4)]
        sets = []
        for want in (False, True):
            sel = np.nonzero(masked == want)[0]
            keys, inv = np.unique(hashes[sel], return_inverse=True)
            sets.append((sel, keys, inv, [np.full(len(keys), -1, np.int64)
                                          for _ in range(4)]))
        for fi in range(len(self.names)):
            raw = self.data(fi)
            h_raw = window_hashes(raw)
            for want, (sel, keys, inv, res) in zip((False, True), sets):
                if not len(keys) or not len(h_raw):
                    continue
                h = h_raw
                if want and self.pointers(fi).any():
                    h = window_hashes(np.where(self.pointers(fi), 0, raw)
                                      .astype(np.uint8))
                idx = np.searchsorted(keys, h)
                idx[idx == len(keys)] = 0
                found = np.nonzero(keys[idx] == h)[0]
                if not len(found):
                    continue
                ki = idx[found]
                res[2][ki] = fi
                res[3][ki] = found
                new = res[0][ki] < 0
                ki, offs = ki[new], found[new]
                # first occurrence wins: assign in reverse order
                res[0][ki[::-1]] = fi
                res[1][ki[::-1]] = offs[::-1]
        for sel, keys, inv, res in sets:
            for o, r in zip(out, res):
                o[sel] = r[inv]
        return out


# ---------------------------------------------------------- matching ------


def match_length(data, reloc, free, d, dptr):
    """length of the run from the start where every byte either equals the
    disc and is free, or is a relocated word facing a PSX pointer"""
    n = min(len(data), len(d))
    pos = 0
    chunk = 64
    while pos < n:
        c = min(chunk, n - pos)
        s = slice(pos, pos + c)
        good = (free[s] & (data[s] == d[s])) | (reloc[s] & dptr[s])
        bad = np.nonzero(~good)[0]
        if len(bad):
            return pos + int(bad[0])
        pos += c
        chunk = min(chunk * 4, 1 << 20)
    return n


class Area:
    """One stretch of ELF data to match: bytes, eligible mask, relocs"""

    def __init__(self, name, addr, data, eligible, reloc):
        self.name = name
        self.addr = addr
        self.data = data
        self.eligible = eligible
        self.reloc = reloc
        self.covered = np.zeros(len(data), bool)


def extend(ar, free, p, d, dptr, off):
    """(start, end) of the match through ELF offset p and disc offset off"""
    if off < 0 or off >= len(d):
        return p, p
    n = match_length(ar.data[p:], ar.reloc[p:], free[p:], d[off:], dptr[off:])
    b = match_length(ar.data[:p][::-1], ar.reloc[:p][::-1], free[:p][::-1],
                     d[:off][::-1], dptr[:off][::-1])
    s, e = p - b, p + n
    # pointers are not part of a match
    while s < e and ar.reloc[s]:
        s += 1
    while e > s and ar.reloc[e - 1]:
        e -= 1
    return s, e


def find_matches(areas, disc, log=print, mask=True):
    """[(area, start, length, file, file offset)] of the disc runs.

    A match is a stretch of the ELF data where every relocated word faces a
    PSX pointer on the disc and every other byte equals the disc, holding at
    least K bytes that are not pointers. It is returned as the pieces between
    the pointers."""
    hashes, where, has_ptr = [], [], []
    for ai, ar in enumerate(areas):
        masked = np.where(ar.reloc, 0, ar.data).astype(np.uint8)
        n = len(ar.data) - K + 1
        if n <= 0:
            continue
        c = np.concatenate([[0], np.cumsum(ar.eligible, dtype=np.int64)])
        n_elig = c[K:K + n] - c[:n]
        ok = (window_ok(ar.eligible | ar.reloc) & (n_elig >= K // 2)
              & ar.eligible[:n] & ~low_entropy(masked))
        pos = np.nonzero(ok)[0]
        if len(pos):
            hashes.append(window_hashes(masked)[pos])
            where.append(np.stack([np.full(len(pos), ai), pos], 1))
            # windows holding pointers are compared with the disc pointers
            # masked, the others with the raw disc (data that keeps PSX
            # addresses as plain numbers)
            has_ptr.append(n_elig[pos] < K)
    if not hashes:
        return []
    hashes = np.concatenate(hashes)
    where = np.concatenate(where)
    has_ptr = np.concatenate(has_ptr) & mask
    log(f"looking up {len(hashes)} windows in {len(disc.names)} disc files")
    ff, fo, lf, lo = disc.lookup(hashes, has_ptr)
    hits = np.nonzero(ff >= 0)[0]
    log(f"{len(hits)} windows found on the disc")

    regions = []
    for ai, ar in enumerate(areas):
        sel = hits[where[hits, 0] == ai]
        pos_list = where[sel, 1]
        order = np.argsort(pos_list, kind="stable")
        sel, pos_list = sel[order], pos_list[order]
        free = ar.eligible.copy()
        prev = None  # (end, file, disc offset of end) of the last match
        for p, hi in zip(pos_list.tolist(), sel.tolist()):
            if not free[p]:
                continue
            cands = []
            if prev and p >= prev[0]:
                cands.append((prev[1], prev[2] + p - prev[0]))
            cands.append((int(ff[hi]), int(fo[hi])))
            cands.append((int(lf[hi]), int(lo[hi])))
            best = None
            for fi, off in dict.fromkeys(cands):
                d, dptr = disc.data(fi), disc.pointers(fi)
                s, e = extend(ar, free, p, d, dptr, off)
                n = int(ar.eligible[s:e].sum())
                if n >= K and (not best or n > best[0]):
                    best = (n, s, e, fi, off - (p - s))
            if not best:
                continue  # hash collision, or overlaps a previous match
            _, s, e, fi, doff = best
            # the pieces between the pointers
            el = ar.eligible[s:e].astype(np.int8)
            edges = np.diff(np.concatenate([[0], el, [0]]))
            starts = np.nonzero(edges == 1)[0]
            ends = np.nonzero(edges == -1)[0]
            for a, b in zip(starts.tolist(), ends.tolist()):
                regions.append((ai, s + a, b - a, fi, doff + a))
            ar.covered[s:e] |= ar.eligible[s:e]
            free[s:e] = False
            prev = (e, fi, doff + (e - s))
    if mask:
        n0 = len(regions)
        fill_gaps(areas, disc, regions)
        log(f"{len(regions) - n0} gaps filled")
    return regions


def fill_gaps(areas, disc, regions):
    """Second pass for the stretches left between matches, mostly runs of
    a repeated value (rows of the same tile, ...) that cannot start a match:
    look the whole stretch, or its longest prefix, up on the disc, first in
    the files of the neighbouring matches."""
    near = {}
    for ai, s, ln, fi, fo in regions:
        near.setdefault(ai, []).append((s, fi))
    raw = [bytes(disc.data(i)) for i in range(len(disc.names))]
    blob = b"".join(raw)
    bounds = np.cumsum([0] + [len(r) for r in raw])
    cache = {}

    def find_all(needle):
        o = blob.find(needle)
        while o >= 0:
            fi = int(np.searchsorted(bounds, o, side="right")) - 1
            if o + len(needle) <= bounds[fi + 1]:
                return fi, o - int(bounds[fi])
            o = blob.find(needle, o + 1)
        return None

    def find(needle, files):
        key = (needle, files)
        if key not in cache:
            hit = None
            if files is None:
                hit = find_all(needle)
            for fi in files or ():
                o = raw[fi].find(needle)
                if o >= 0:
                    hit = (fi, o)
                    break
            cache[key] = hit
        return cache[key]

    all_files = None
    for ai, ar in enumerate(areas):
        starts = sorted(near.get(ai, []))
        keys = [s for s, _ in starts]
        left = ar.eligible & ~ar.covered
        edges = np.diff(np.concatenate([[0], left.astype(np.int8), [0]]))
        for a, b in zip(np.nonzero(edges == 1)[0].tolist(),
                        np.nonzero(edges == -1)[0].tolist()):
            tries = 4  # starts of the stretch looked up when nothing matches
            while b - a >= K:
                chunk = bytes(ar.data[a:b])
                if not any(chunk):
                    break
                i = np.searchsorted(keys, a)
                files = tuple(dict.fromkeys(
                    starts[j][1] for j in (i - 1, i) if 0 <= j < len(starts)))
                hit = None
                for cand in (files, all_files):
                    hit = find(chunk, cand)
                    if hit:
                        n = b - a
                        break
                    if not find(chunk[:K], cand):
                        continue
                    lo, hi = K, b - a  # longest prefix found on the disc
                    while lo < hi:
                        mid = (lo + hi + 1) // 2
                        if find(chunk[:mid], cand):
                            lo = mid
                        else:
                            hi = mid - 1
                    hit = find(chunk[:lo], cand)
                    n = lo
                    break
                if not hit:
                    tries -= 1
                    if not tries:
                        break
                    a += 4
                    continue
                fi, fo = hit
                regions.append((ai, a, n, fi, fo))
                ar.covered[a:a + n] = True
                a += n


# ---------------------------------------------------------- manifest ------
#
# Little endian, read by vita_assets.c:
#   header:   magic[8] version build_id[16] n_files n_anchors n_regions
#             n_sections total_len total_crc
#   files:    size u32, name_len u16, name (relative to disks/us)
#   anchors:  link-time address u32 (0: g_VitaAssetsBuildId)
#   regions:  target offset from its anchor u32, len u32, file offset u32,
#             crc32 u32, file u16, anchor u8, pad u8
#   sections: link-time address u32, size u32 of the data sections, only used
#             by the debug dump
# Regions are sorted by file then file offset. total_crc is the CRC32 of the
# bytes of all the regions concatenated in that order.

HEADER = struct.Struct("<8sI16sIIIIII")
REGION = struct.Struct("<IIIIHBB")


def write_manifest(bid, disc, anchors, regions, sections):
    used = sorted({r[2] for r in regions})
    remap = {fi: i for i, fi in enumerate(used)}
    total_crc = 0
    for (tgt, ln, fi, fo, crc, anchor) in regions:
        total_crc = zlib.crc32(bytes(disc.data(fi)[fo:fo + ln]), total_crc)
    out = bytearray(HEADER.pack(
        MANIFEST_MAGIC, MANIFEST_VERSION, bid, len(used), len(anchors),
        len(regions), len(sections), sum(r[1] for r in regions), total_crc))
    for fi in used:
        name = disc.names[fi].encode()
        out += struct.pack("<IH", len(disc.data(fi)), len(name)) + name
    for a in anchors:
        out += struct.pack("<I", a)
    for (tgt, ln, fi, fo, crc, anchor) in regions:
        out += REGION.pack(tgt, ln, fo, crc, remap[fi], anchor, 0)
    for addr, size in sections:
        out += struct.pack("<II", addr, size)
    return bytes(out)


def read_manifest(path):
    with open(path, "rb") as f:
        raw = f.read()
    (magic, version, bid, n_files, n_anchors, n_regions, n_sections,
     total_len, total_crc) = HEADER.unpack_from(raw, 0)
    if magic != MANIFEST_MAGIC or version != MANIFEST_VERSION:
        sys.exit(f"{path}: not a manifest of version {MANIFEST_VERSION}")
    pos = HEADER.size
    files = []
    for _ in range(n_files):
        size, nlen = struct.unpack_from("<IH", raw, pos)
        pos += 6
        files.append((raw[pos:pos + nlen].decode(), size))
        pos += nlen
    anchors = list(struct.unpack_from(f"<{n_anchors}I", raw, pos))
    pos += 4 * n_anchors
    regions = [REGION.unpack_from(raw, pos + i * REGION.size)
               for i in range(n_regions)]
    pos += REGION.size * n_regions
    sections = [struct.unpack_from("<II", raw, pos + i * 8)
                for i in range(n_sections)]
    if pos + 8 * n_sections != len(raw):
        sys.exit(f"{path}: bad size")
    return dict(version=version, build_id=bid, files=files, anchors=anchors,
                regions=regions, sections=sections, total_len=total_len,
                total_crc=total_crc)


# ---------------------------------------------------------- commands ------


def keep_ranges(map_path, keep_patterns):
    keep = [re.compile(p) for p in keep_patterns]
    ranges = []
    objs = set()
    merged = set()
    for addr, size, obj, pooled, sec in parse_map(map_path):
        kept = any(k.search(obj) for k in keep)
        if size and kept:
            ranges.append((addr, addr + size))
            objs.add(obj)
        if pooled and not kept and sec.startswith((".rodata", ".data")):
            merged.add(obj)
    if merged:
        # their constants sit in a pool listed under a toolchain object,
        # which would keep them
        sys.exit(f"{len(merged)} objects of the project have merged "
                 f"constants (e.g. {sorted(merged)[0]}): build them with "
                 "-fno-merge-constants")
    return ranges, objs


def make_areas(elf, keep):
    areas = []
    for sec in data_sections(elf):
        data = elf.section_bytes(sec)
        reloc = np.zeros(len(data), bool)
        for r in elf.reloc_slots(sec).tolist():
            o = r - sec.addr
            if 0 <= o < len(data):
                reloc[o:o + 4] = True
        eligible = ~reloc
        for lo, hi in keep:
            a, b = max(lo, sec.addr), min(hi, sec.addr + sec.size)
            if a < b:
                eligible[a - sec.addr:b - sec.addr] = False
        areas.append(Area(sec.name, sec.addr, data, eligible, reloc))
    return areas


def generated_symbols(root):
    """names of the arrays and structs defined by disc-generated sources"""
    files = []
    for dirpath, dirs, fns in os.walk(os.path.join(root, "src")):
        if "gen" in dirpath.replace(os.sep, "/").split("/"):
            files += [os.path.join(dirpath, f) for f in fns]
    gi = os.path.join(root, ".gitignore")
    if os.path.exists(gi):
        with open(gi) as f:
            in_pc = False
            for line in f:
                line = line.strip()
                if "tools/vita/generate_pc_data.sh" in line:
                    in_pc = True
                    continue
                if in_pc and line.startswith("/src/"):
                    pat = line[1:]
                    d = os.path.join(root, os.path.dirname(pat))
                    rx = re.compile("^" + re.escape(os.path.basename(pat))
                                    .replace(r"\*", ".*") + "$")
                    if os.path.isdir(d):
                        files += [os.path.join(d, f) for f in os.listdir(d)
                                  if rx.match(f)]
                elif in_pc and not line:
                    in_pc = False
    defn = re.compile(r"^\s*(?:static\s+)?(?:const\s+)?[A-Za-z_][\w\s\*]*?\b"
                      r"([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*=\s*\{", re.M)
    names = {}
    for path in files:
        try:
            with open(path, errors="replace") as f:
                text = f.read()
        except OSError:
            continue
        rel = os.path.relpath(path, root)
        for m in defn.finditer(text):
            names.setdefault(m.group(1), rel)
    return names


def cmd_blank(args):
    log = print
    elf = Elf(args.elf)
    disc = Disc(args.disc)
    keep, keep_objs = ([], set())
    if args.map:
        keep, keep_objs = keep_ranges(args.map, DEFAULT_KEEP + args.keep)
    else:
        log("warning: no link map, the toolchain data may be blanked too")
    areas = make_areas(elf, keep)
    eligible = sum(int(a.eligible.sum()) for a in areas)
    log(f"{len(areas)} data sections, {eligible} eligible bytes, "
        f"{len(keep_objs)} objects kept")
    matches = find_matches(areas, disc, log)
    if args.dry_run:
        for ai, s, ln, fi, fo in matches:
            areas[ai].covered[s:s + ln] = True
        regions = [(0, ln, fi, fo, 0, 0) for ai, s, ln, fi, fo in matches]
        report = make_report(elf, areas, regions, disc, keep_objs, args)
        if args.report:
            with open(args.report, "w") as f:
                f.write(report)
        log(report.split("\n\n")[0])
        for name in args.explain:
            explain(elf, areas, matches, disc, name)
        return

    bid_sym = elf.symbol(BUILD_ID_SYMBOL)
    anchor_rw = bid_sym[1]
    out = Elf(args.elf)  # the copy to blank
    regions = []
    for ai, s, ln, fi, fo in matches:
        ar = areas[ai]
        vaddr = ar.addr + s
        if segment_of(elf, vaddr) != segment_of(elf, anchor_rw):
            sys.exit(f"{ar.name}+{s:#x}: disc data outside the data segment, "
                     "see vita_writable_rodata() in tools/vita/vita_link.cmake")
        chunk = bytes(ar.data[s:s + ln])
        if chunk != bytes(disc.data(fi)[fo:fo + ln]):
            sys.exit("internal error: region does not match the disc")
        regions.append(((vaddr - anchor_rw) & 0xFFFFFFFF, ln, fi, fo,
                        zlib.crc32(chunk), 0))
        off = elf.vaddr_to_offset(vaddr, ln)
        out.raw[off:off + ln] = bytes(ln)
    regions.sort(key=lambda r: (r[2], r[3], r[0]))

    bid = build_id(elf)
    off = elf.vaddr_to_offset(anchor_rw, 16)
    if bytes(elf.raw[off:off + 16]) != BUILD_ID_MAGIC:
        sys.exit(f"{BUILD_ID_SYMBOL} does not hold the expected placeholder")
    out.raw[off:off + 16] = bid

    sections = [(s.addr, s.size) for s in data_sections(elf)]
    man = write_manifest(bid, disc, [anchor_rw], regions, sections)
    with open(args.manifest, "wb") as f:
        f.write(man)
    with open(args.out_elf, "wb") as f:
        f.write(out.raw)
    os.chmod(args.out_elf, 0o755)

    report = make_report(elf, areas, regions, disc, keep_objs, args)
    if args.report:
        with open(args.report, "w") as f:
            f.write(report)
    log(report.split("\n\n")[0])
    log(f"manifest {args.manifest}: {len(man)} bytes, build id {bid.hex()}")


def explain(elf, areas, matches, disc, name):
    """debug aid: the matches and the bytes left of a symbol"""
    for sym in [s for s in elf.symbols() if s[0] == name]:
        for ai, ar in enumerate(areas):
            if not ar.addr <= sym[1] < ar.addr + len(ar.data):
                continue
            s0 = sym[1] - ar.addr
            e0 = s0 + sym[2]
            print(f"{name} in {ar.name} +{s0:#x} size {sym[2]:#x}")
            for m in sorted(m for m in matches if m[0] == ai
                            and m[1] < e0 and m[1] + m[2] > s0):
                print(f"  match +{m[1] - s0:#x} len {m[2]:#x} "
                      f"{disc.names[m[3]]}+{m[4]:#x}")
            left = ar.eligible[s0:e0] & ~ar.covered[s0:e0]
            edges = np.diff(np.concatenate([[0], left.astype(np.int8), [0]]))
            for a, b in zip(np.nonzero(edges == 1)[0], np.nonzero(edges == -1)[0]):
                print(f"  left +{a:#x}..+{b:#x}: "
                      f"{bytes(ar.data[s0 + a:s0 + min(b, a + 32)]).hex()}")


def make_report(elf, areas, regions, disc, keep_objs, args):
    lines = []
    blanked = sum(r[1] for r in regions)
    nz = 0
    for ar in areas:
        nz += int(np.count_nonzero(ar.data[ar.covered]))
    total = sum(len(a.data) for a in areas)
    files = sorted({disc.names[r[2]] for r in regions})
    lines.append(f"data bytes: {total}, blanked: {blanked} "
                 f"({100.0 * blanked / max(total, 1):.1f}%), non-zero blanked: "
                 f"{nz}, regions: {len(regions)}, disc files used: {len(files)}")
    lines.append("")
    lines.append("per section (bytes, blanked, eligible not blanked & non-zero):")
    for ar in areas:
        left = ar.eligible & ~ar.covered & (ar.data != 0)
        if ar.covered.any() or left.sum() > 64:
            lines.append(f"  {ar.name:28s} {len(ar.data):9d} "
                         f"{int(ar.covered.sum()):9d} {int(left.sum()):9d}")
    lines.append("")
    lines.append("disc files used: " + " ".join(files))
    lines.append("")
    lines.append("objects kept (data never blanked):")
    for o in sorted(keep_objs):
        lines.append(f"  {o}")
    lines.append("")

    # symbols with data that is neither blanked nor a pointer
    gen = generated_symbols(args.root) if args.root else {}
    by_addr = []
    for ar in areas:
        by_addr.append((ar.addr, ar.addr + len(ar.data), ar))
    rows = []
    for name, value, size, typ, shndx in elf.symbols():
        if typ != 1 or size < 16:
            continue
        for lo, hi, ar in by_addr:
            if lo <= value < hi:
                s = value - lo
                e = min(s + size, len(ar.data))
                left = (ar.eligible[s:e] & ~ar.covered[s:e]
                        & (ar.data[s:e] != 0))
                n = int(left.sum())
                if n >= 16:
                    rows.append((n, name, size, int(ar.covered[s:e].sum()),
                                 ar.name, gen.get(name, "")))
                break
    rows.sort(reverse=True)
    g = [r for r in rows if r[5]]
    lines.append(f"generated symbols with non-zero bytes not found on the "
                 f"disc ({len(g)}): left bytes, name, size, blanked, section, "
                 "source")
    for r in g:
        lines.append(f"  {r[0]:7d} {r[1]:40s} {r[2]:7d} {r[3]:7d} {r[4]} {r[5]}")
    lines.append("")
    # same-named local symbols (one per overlay, __func__, ...) together
    other = {}
    for r in rows:
        if not r[5]:
            o = other.setdefault(r[1], [0, 0, 0, 0, set()])
            o[0] += r[0]
            o[1] += r[2]
            o[2] += r[3]
            o[3] += 1
            o[4].add(r[4])
    lines.append(f"other symbols with non-zero bytes not found on the disc "
                 f"({len(rows) - len(g)}), largest first: left bytes, name "
                 "(copies), size, blanked, sections")
    for name, o in sorted(other.items(), key=lambda x: -x[1][0])[:args.report_max]:
        secs = " ".join(sorted(o[4])[:3]) + (" ..." if len(o[4]) > 3 else "")
        lines.append(f"  {o[0]:7d} {name + f' ({o[3]})':40s} {o[1]:7d} "
                     f"{o[2]:7d} {secs}")
    return "\n".join(lines) + "\n"


def restore_image(elf_path, man_path, disc_root):
    """the blanked ELF restored from the manifest like vita_assets.c does"""
    elf = Elf(elf_path)
    man = read_manifest(man_path)
    anchors = man["anchors"]
    crc_all = 0
    total = 0
    cache = {}
    for (tgt, ln, fo, crc, fidx, anchor, _) in man["regions"]:
        name, size = man["files"][fidx]
        if name not in cache:
            with open(os.path.join(disc_root, name), "rb") as f:
                cache[name] = f.read()
            if len(cache[name]) != size:
                sys.exit(f"{name}: size {len(cache[name])}, expected {size}")
        chunk = cache[name][fo:fo + ln]
        if zlib.crc32(chunk) != crc:
            sys.exit(f"{name}+{fo:#x}: checksum mismatch")
        crc_all = zlib.crc32(chunk, crc_all)
        total += ln
        vaddr = (anchors[anchor] + tgt) & 0xFFFFFFFF
        off = elf.vaddr_to_offset(vaddr, ln)
        if any(elf.raw[off:off + ln]):
            sys.exit(f"{vaddr:#x}: target is not blank")
        elf.raw[off:off + ln] = chunk
    if crc_all != man["total_crc"] or total != man["total_len"]:
        sys.exit("total checksum mismatch")
    return elf, man


def cmd_verify(args):
    real = Elf(args.elf)
    restored, man = restore_image(args.blank_elf, args.manifest, args.disc)
    bid_sym = real.symbol(BUILD_ID_SYMBOL)
    off = real.vaddr_to_offset(bid_sym[1], 16)
    if bytes(restored.raw[off:off + 16]) != man["build_id"]:
        sys.exit("the blanked ELF does not carry the manifest build id")
    if man["build_id"] != build_id(real):
        sys.exit("the manifest does not belong to this ELF")
    restored.raw[off:off + 16] = real.raw[off:off + 16]
    bad = 0
    for a, b in zip(real.load_segments(), restored.load_segments()):
        if a != b:
            x = np.frombuffer(a[1], np.uint8)
            y = np.frombuffer(b[1], np.uint8)
            diff = np.nonzero(x != y)[0]
            print(f"segment {a[0]:#x}: {len(diff)} bytes differ, first at "
                  f"{a[0] + int(diff[0]):#x}")
            bad += 1
    n = len(man["regions"])
    print(f"restored {n} regions, {man['total_len']} bytes from "
          f"{len(man['files'])} disc files")
    if bad:
        sys.exit(1)
    print("OK: restored image equals the original ELF")


def self_segments(path):
    """[(vaddr, bytes)] of the loaded segments of an ELF or SELF file"""
    with open(path, "rb") as f:
        raw = f.read()
    if raw[:4] == b"\x7fELF":
        return Elf(path).load_segments()
    if raw[:4] != b"SCE\0":
        sys.exit(f"{path}: neither ELF nor SELF")
    # SCE header then the extended SELF header (vita-toolchain self.h)
    ext = struct.unpack_from("<12Q", raw, 32)
    elf_off, phdr_off, secinfo_off = ext[4], ext[5], ext[7]
    phnum = struct.unpack_from("<H", raw, elf_off + 44)[0]
    out = []
    for i in range(phnum):
        ph = struct.unpack_from("<8I", raw, phdr_off + i * 32)
        offset, length, compression, encryption = struct.unpack_from(
            "<4Q", raw, secinfo_off + i * 32)
        if ph[0] != PT_LOAD or not length:
            continue
        data = raw[offset:offset + length]
        if compression == 2:
            data = zlib.decompress(data)
        if len(data) != ph[4]:
            sys.exit(f"{path}: segment {i} has {len(data)} bytes, "
                     f"expected {ph[4]}")
        out.append((ph[2], data))
    return out


def cmd_scan(args):
    disc = Disc(args.disc)
    keep = []
    if args.map:
        keep = [(a, a + n, o) for a, n, o, _, _ in parse_map(args.map)
                if n and any(re.search(k, o) for k in DEFAULT_KEEP + args.keep)]
    syms = []
    if args.elf:
        syms = sorted((v, n, sz) for n, v, sz, t, _ in Elf(args.elf).symbols()
                      if t == 1 and sz)
    keys = [v for v, _, _ in syms]

    def where(addr):
        name = ""
        i = int(np.searchsorted(keys, addr, side="right")) - 1
        if i >= 0 and syms[i][0] <= addr < syms[i][0] + syms[i][2]:
            name = syms[i][1]
        for lo, hi, obj in keep:
            if lo <= addr < hi:
                return name, os.path.basename(obj)
        return name, None

    for path in args.files:
        areas = []
        for vaddr, data in self_segments(path):
            a = np.frombuffer(data, np.uint8)
            areas.append(Area(f"{vaddr:#x}", vaddr, a, np.ones(len(a), bool),
                              np.zeros(len(a), bool)))
        total = sum(len(a.data) for a in areas)
        print(f"{path}: scanning {total} bytes in {len(areas)} segments")
        regions = find_matches(areas, disc, log=lambda *a: None, mask=False)
        # The blanked regions are zeros, which match the zero padding of the
        # disc files: a run only counts for its non-zero bytes
        bad = []
        kept = 0
        weak = 0
        for ai, s, ln, fi, fo in regions:
            run = areas[ai].data[s:s + ln]
            nz = np.nonzero(run)[0]
            if len(nz) < K:
                weak += 1
                continue
            s, fo, ln = s + int(nz[0]), fo + int(nz[0]), int(nz[-1] - nz[0]) + 1
            addr = areas[ai].addr + s
            name, obj = where(addr)
            if obj:
                kept += 1
            else:
                bad.append((len(nz), addr, ln, fi, fo, name))
        bad.sort(reverse=True)
        for nz, addr, ln, fi, fo, name in bad[:args.max]:
            print(f"  {addr:#x} len {ln} ({nz} non-zero) {name} = "
                  f"{disc.names[fi]}+{fo:#x}")
        print(f"  runs of {K}+ bytes found on the disc: {len(regions)}; "
              f"{weak} with less than {K} non-zero bytes, {kept} in "
              f"toolchain/backend objects, {len(bad)} elsewhere "
              f"({sum(b[0] for b in bad)} non-zero bytes)")
        if args.verbose:
            for ai, s, ln, fi, fo in regions:
                addr = areas[ai].addr + s
                name, obj = where(addr)
                nz = int(np.count_nonzero(areas[ai].data[s:s + ln]))
                if obj and nz >= K:
                    print(f"    kept {addr:#x} len {ln} ({nz} non-zero) "
                          f"{name} ({obj}) = {disc.names[fi]}+{fo:#x}")
        for want in args.absent or []:
            name, _, fo_len = want.partition("+")
            fo, _, ln = fo_len.partition(":")
            fo, ln = int(fo, 0), int(ln, 0)
            with open(os.path.join(args.disc, name), "rb") as f:
                f.seek(fo)
                needle = f.read(ln)
            found = [a.name for a in areas if needle in bytes(a.data)]
            nz = sum(1 for x in needle if x)
            print(f"  spot check {name}+{fo:#x} ({ln} bytes, {nz} non-zero): "
                  + (f"FOUND in segment {found}" if found else "absent"))
            if found:
                bad.append(None)
        if bad and args.strict:
            sys.exit(f"{path}: disc data found")


def cmd_check_dump(args):
    """Compares the data sections dumped by the game right after the restore
    with the original ELF. Relocated words are skipped. The restored regions
    must be identical; other differences come from code that ran before the
    dump (C library, Vita backend) and are listed by symbol."""
    real = Elf(args.elf)
    man = read_manifest(args.manifest)
    with open(args.dump, "rb") as f:
        raw = f.read()
    if raw[:8] != b"SOTNDUMP":
        sys.exit(f"{args.dump}: not a dump")
    rt_anchor, count = struct.unpack_from("<II", raw, 8)
    print(f"data segment relocated by "
          f"{(rt_anchor - man['anchors'][0]) & 0xFFFFFFFF:#x}")
    sections = {s.addr: s for s in data_sections(real)}
    restored_bytes = 0
    bad_restored = 0
    other = {}
    pos = 16
    for _ in range(count):
        link, ln = struct.unpack_from("<II", raw, pos)
        pos += 8
        rt = np.frombuffer(raw[pos:pos + ln], np.uint8).copy()
        pos += ln
        sec = sections[link]
        ref = real.section_bytes(sec)
        for r in real.reloc_slots(sec).tolist():
            o = r - link
            rt[o:o + 4] = ref[o:o + 4]
        restored = np.zeros(ln, bool)
        for (tgt, rln, fo, crc, fidx, anchor, _) in man["regions"]:
            v = (man["anchors"][anchor] + tgt) & 0xFFFFFFFF
            if link <= v < link + ln:
                restored[v - link:v - link + rln] = True
        restored_bytes += int(restored.sum())
        diff = rt != ref
        bad_restored += int((diff & restored).sum())
        for d in np.nonzero(diff & ~restored)[0].tolist():
            other[link + d] = True
    print(f"restored regions: {restored_bytes} bytes compared, "
          f"{bad_restored} differ")
    if other:
        syms = sorted((v, n, sz) for n, v, sz, t, _ in real.symbols()
                      if t == 1 and sz)
        keys = [v for v, _, _ in syms]
        by_sym = {}
        for addr in other:
            i = int(np.searchsorted(keys, addr, side="right")) - 1
            name = "?"
            if i >= 0 and syms[i][0] <= addr < syms[i][0] + syms[i][2]:
                name = syms[i][1]
            by_sym[name] = by_sym.get(name, 0) + 1
        print(f"bytes changed at runtime outside the restored regions: "
              f"{len(other)} in {len(by_sym)} symbols")
        for name, n in sorted(by_sym.items(), key=lambda x: -x[1])[:40]:
            print(f"  {n:7d} {name}")
    if bad_restored:
        sys.exit("FAIL: restored data differs from the original ELF")
    print("OK: every restored byte equals the original ELF")


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("blank")
    p.add_argument("--elf", required=True)
    p.add_argument("--disc", required=True)
    p.add_argument("--map")
    p.add_argument("--keep", action="append", default=[],
                   help="regex of link map objects whose data is kept")
    p.add_argument("--out-elf", required=True)
    p.add_argument("--manifest", required=True)
    p.add_argument("--report")
    p.add_argument("--report-max", type=int, default=200)
    p.add_argument("--dry-run", action="store_true",
                   help="only report, write nothing but the report")
    p.add_argument("--explain", action="append", default=[],
                   help="with --dry-run: show the matches of a symbol")
    p.add_argument("--root", default=".", help="repository root")
    p = sub.add_parser("verify")
    p.add_argument("--elf", required=True)
    p.add_argument("--blank-elf", required=True)
    p.add_argument("--manifest", required=True)
    p.add_argument("--disc", required=True)
    p = sub.add_parser("scan")
    p.add_argument("--disc", required=True)
    p.add_argument("--elf", help="ELF whose symbols name the matches")
    p.add_argument("--map", help="link map: matches in the toolchain or "
                   "Vita backend objects are only counted")
    p.add_argument("--keep", action="append", default=[])
    p.add_argument("-v", "--verbose", action="store_true")
    p.add_argument("--max", type=int, default=40)
    p.add_argument("--strict", action="store_true",
                   help="exit with an error when anything is found")
    p.add_argument("--absent", action="append",
                   help="FILE+OFFSET:LEN of the disc that must not appear")
    p.add_argument("files", nargs="+")
    p = sub.add_parser("check-dump")
    p.add_argument("--elf", required=True)
    p.add_argument("--manifest", required=True)
    p.add_argument("dump")
    args = ap.parse_args()
    {"blank": cmd_blank, "verify": cmd_verify, "scan": cmd_scan,
     "check-dump": cmd_check_dump}[args.cmd](args)


if __name__ == "__main__":
    main()
