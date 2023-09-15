#syntax=docker/dockerfile:1
FROM ubuntu:22.04 AS base

ENV DEBIAN_FRONTEND=noninteractive
ENV BUILD_PACKAGES \
    make cmake g++ libboost-program-options-dev \
    libboost-math-dev libboost-random-dev libboost-filesystem-dev \
    libboost-date-time-dev

ENV RUNTIME_PACKAGES \
    libboost-filesystem1.74.0 libboost-program-options1.74.0

ENV DEBUGGER_PACKAGES \
    gdb gdbserver gdb gdbserver ccache python3 linux-tools-generic-hwe-22.04 linux-tools-generic systemtap-sdt-dev

# SSH dependencies
ENV SSH_DEPENDENCIES openssh-server sudo rsync

RUN --mount=target=/var/lib/apt/lists,type=cache,sharing=locked \
    --mount=target=/var/cache/apt,type=cache,sharing=locked \
    rm -f /etc/apt/apt.conf.d/docker-clean && \
    apt update && \
    apt install -y --no-install-recommends $BUILD_PACKAGES $RUNTIME_PACKAGES

RUN --mount=target=/var/lib/apt/lists,type=cache,sharing=locked \
    --mount=target=/var/cache/apt,type=cache,sharing=locked \
    rm -f /etc/apt/apt.conf.d/docker-clean && \
    apt install -y --no-install-recommends $DEBUGGER_PACKAGES $BUILD_PACKAGES $RUNTIME_PACKAGES $SSH_DEPENDENCIES

RUN echo kernel.perf_event_paranoid=1 >> /etc/sysctl.d/99-perf.conf && \
    echo kernel.kptr_restrict=0 >> /etc/sysctl.d/99-perf.conf && \
    sysctl --system

RUN rm /usr/bin/perf && \
    ln -s /usr/lib/linux-tools/5.19.0-46-generic/perf /usr/bin/perf

RUN mkdir /var/run/sshd && \
    echo 'root:debugger' | chpasswd && \
    echo 'PermitRootLogin yes' >> /etc/ssh/sshd_config && \
    sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

EXPOSE 22

CMD ["/usr/sbin/sshd","-D"]
