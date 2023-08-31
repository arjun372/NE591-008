#syntax=docker/dockerfile:1
FROM ubuntu:22.04 AS base

ENV DEBIAN_FRONTEND=noninteractive
ENV BUILD_PACKAGES \
    make cmake g++ libboost-program-options-dev \
    libboost-math-dev libboost-random-dev libboost-filesystem-dev \
    libboost-date-time-dev

ENV RUNTIME_PACKAGES \
    libboost-filesystem1.74.0 libboost-program-options1.74.0

RUN --mount=target=/var/lib/apt/lists,type=cache,sharing=locked \
    --mount=target=/var/cache/apt,type=cache,sharing=locked \
    rm -f /etc/apt/apt.conf.d/docker-clean && \
    apt update && \
    apt install -y --no-install-recommends $BUILD_PACKAGES $RUNTIME_PACKAGES

FROM base AS debugger
ENV DEBUGGER_PACKAGES \
    gdb linux-tools-generic-hwe-22.04 linux-tools-generic systemtap-sdt-dev
RUN --mount=target=/var/lib/apt/lists,type=cache,sharing=locked \
    --mount=target=/var/cache/apt,type=cache,sharing=locked \
    rm -f /etc/apt/apt.conf.d/docker-clean && \
    apt install -y --no-install-recommends $DEBUGGER_PACKAGES

RUN echo kernel.perf_event_paranoid=1 >> /etc/sysctl.d/99-perf.conf && \
    echo kernel.kptr_restrict=0 >> /etc/sysctl.d/99-perf.conf && \
    sysctl --system

RUN rm /usr/bin/perf && \
    ln -s /usr/lib/linux-tools/5.19.0-46-generic/perf /usr/bin/perf

ARG UID=1000
RUN useradd -m -u ${UID} -s /bin/bash debugger
USER debugger
ENTRYPOINT ["/bin/bash"]

FROM base AS builder
WORKDIR /app
COPY . .
# Build type
ARG CMAKE_BUILD_TYPE=Debug
# Build the tests
ARG CMAKE_BUILD_TESTS=ON
# Build the tests
ARG CMAKE_BUILD_PERFORMANCE_BENCHMARKS=ON
# Optimize for native builds, trading portability for performance
ARG CMAKE_OPTIMIZE_FOR_NATIVE=ON

WORKDIR /app/build
RUN cmake .. \
    -D CMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
    -D BUILD_TESTS=$CMAKE_BUILD_TESTS \
    -D BUILD_PERFORMANCE_BENCHMARKS=$CMAKE_BUILD_PERFORMANCE_BENCHMARKS \
    -D OPTIMIZE_FOR_NATIVE=$CMAKE_OPTIMIZE_FOR_NATIVE \
    && make -j$(nproc)
