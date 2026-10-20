# syntax=docker/dockerfile:1
# Build environment for the PS Vita port: Ubuntu, the host tools used by the
# decomp (go, cargo, python), the VitaSDK and the Vita libraries the build
# links. The repository is copied in last; the disc image is never part of
# the image, it is mounted at run time.
#
# Build (from the repository root):
#   docker build -f tools/vita/docker/Dockerfile -t sotn-vita-kit .
# Run:
#   docker run --rm -v <disc dir>:/rom:ro -v <output dir>:/out \
#       sotn-vita-kit "/rom/<name>.cue" /out
#
# tools/vita/build-vita.ps1 and tools/vita/build-vita.sh do both steps.

ARG UBUNTU_TAG=26.04
FROM ubuntu:${UBUNTU_TAG}

ENV DEBIAN_FRONTEND=noninteractive \
    LANG=C.UTF-8 \
    LC_ALL=C.UTF-8

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential bzip2 ca-certificates cargo cmake curl file git \
        golang-go make ninja-build p7zip-full pkg-config python3 python3-dev \
        python3-pip python3-venv rsync rustc wget xz-utils && \
    rm -rf /var/lib/apt/lists/*

# vdpm client and VitaSDK release series. The series pins the toolchain and
# the package snapshot it ships with. Declared after the apt layer so that
# changing them does not invalidate it.
ARG VDPM_REF=c9ba33bd428ba88052f79d78592f4b318a7a322b
ARG VITASDK_CHANNEL=2026.08
# libvita2d plus the libraries it is built against, and libdl: CMake links
# ${CMAKE_DL_LIBS} on every non-Windows target
ARG VITA_PACKAGES="zlib libpng libjpeg-turbo freetype libvita2d vita-rss-libdl"

ENV VITASDK=/usr/local/vitasdk
ENV PATH=${VITASDK}/bin:${PATH}
# Downloads come from GitHub releases; retry transient network failures.
RUN git clone --quiet https://github.com/vitasdk/vdpm /tmp/vdpm && \
    git -C /tmp/vdpm checkout --quiet ${VDPM_REF} && \
    for try in 1 2 3 4 5 6 7 8; do \
        VITASDK_CHANNEL=${VITASDK_CHANNEL} /tmp/vdpm/bootstrap-vitasdk.sh && break; \
        rm -rf ${VITASDK}; echo "bootstrap failed, retrying ($try)"; sleep $((try * 10)); \
    done && \
    test -x ${VITASDK}/bin/vdpm && \
    for try in 1 2 3 4 5 6 7 8; do \
        VDPM_NONINTERACTIVE=1 vdpm install ${VITA_PACKAGES} && break; \
        echo "vdpm install failed, retrying ($try)"; sleep $((try * 10)); \
    done && \
    test -f ${VITASDK}/arm-vita-eabi/lib/libvita2d.a && \
    test -f ${VITASDK}/arm-vita-eabi/lib/libdl.a && \
    rm -rf /tmp/vdpm ${VITASDK}/var/cache/pacman/pkg/*

WORKDIR /sotn

# Python tools, in the virtualenv the Makefile expects (.venv)
COPY tools/requirements-python.txt /tmp/requirements-python.txt
RUN python3 -m venv /sotn/.venv && \
    /sotn/.venv/bin/pip install --no-cache-dir -r /tmp/requirements-python.txt

# Go module cache for tools/sotn-disk and tools/sotn-assets
ENV GOTOOLCHAIN=local
COPY go.work go.work.sum ./
COPY tools/gfxsotn/go.* tools/gfxsotn/
COPY tools/sotn-assets/go.* tools/sotn-assets/
COPY tools/sotn-disk/go.* tools/sotn-disk/
RUN go mod download all

# Crates for the sotn_str host tool that CMake builds
COPY tools/sotn_str/Cargo.toml tools/sotn_str/Cargo.lock /tmp/sotn_str/
RUN mkdir /tmp/sotn_str/src && echo 'fn main() {}' > /tmp/sotn_str/src/main.rs && \
    cargo fetch --manifest-path /tmp/sotn_str/Cargo.toml && \
    rm -rf /tmp/sotn_str

# The sources. See Dockerfile.dockerignore: build outputs and anything that
# comes from the disc are excluded. Scripts checked out on Windows may carry
# CRLF line endings, which bash and make do not accept.
COPY . /sotn
RUN find /sotn -type f \( -name '*.sh' -o -name 'Makefile' -o -name '*.mk' \) \
        -exec sed -i 's/\r$//' {} + && \
    chmod +x /sotn/sotn.sh /sotn/tools/vita/*.sh

VOLUME ["/out"]
ENTRYPOINT ["bash", "/sotn/tools/vita/build_from_rom.sh"]
CMD ["--help"]
