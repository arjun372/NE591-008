ARG OS_VERSION="22.04"
ARG CUDA_VERSION="12.0.0"
FROM nvidia/cuda:${CUDA_VERSION}-devel-ubuntu${OS_VERSION} as base
ENV DEBIAN_FRONTEND=noninteractive
ENV BUILD_PACKAGES build-essential ca-certificates gdb gfortran wget patchelf strace ltrace

RUN --mount=target=/var/lib/apt/lists,type=cache,sharing=locked \
    --mount=target=/var/cache/apt,type=cache,sharing=locked \
    rm -f /etc/apt/apt.conf.d/docker-clean && \
    apt update && \
    apt install -y --no-install-recommends $BUILD_PACKAGES

FROM base as builder
ENV NVIDIA_VISIBLE_DEVICES=void
ARG GCC_VERSION="10.2.0"
ARG GCC_CONFIGURE_OPTIONS="--prefix=/usr/local --disable-multilib"

RUN wget https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.gz && \
    tar -xzf gcc-${GCC_VERSION}.tar.gz && \
    cd gcc-${GCC_VERSION} && \
    ./contrib/download_prerequisites && \
    ./configure --prefix=/usr/local --disable-multilib && \
    make -j$(nproc) && \
    make install && \
    cd .. && \
    rm -rf gcc-${GCC_VERSION} gcc-${GCC_VERSION}.tar.gz


# Build OPENMPI
ARG MPI_VERSION="4.1.0"
ARG MPI_CONFIGURE_OPTIONS="--enable-fast=all,O3 --prefix=/usr --with-cuda=/usr/local/cuda"
RUN mkdir -p /tmp/openmpi-build \
      && cd /tmp/openmpi-build \
      && MPI_VER_MM="${MPI_VERSION%.*}" \
      && wget http://www.openmpi.org/software/ompi/v${MPI_VER_MM}/downloads/openmpi-${MPI_VERSION}.tar.bz2 \
      && tar xjf openmpi-${MPI_VERSION}.tar.bz2 \
      && cd openmpi-${MPI_VERSION}  \
      && ./configure ${MPI_CONFIGURE_OPTIONS} \
      && make -j$(nproc) \
      && make install \
      && ldconfig \
      && cd / \
      && rm -rf /tmp/openmpi-build

ENV PATH=/usr/local/cuda-12.2/bin:${PATH:+:${PATH}}
ENV LD_LIBRARY_PATH=/usr/local/cuda-12.2/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
ENV LD_LIBRARY_PATH=/usr/local/cuda-12.2/lib${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}

FROM builder as debugger

# Final settings
WORKDIR /
ARG UID=1000
RUN useradd -m -u ${UID} -s /bin/bash debugger && adduser debugger sudo
USER debugger
CMD ["/bin/bash"]

# Create a new stage based on the debugger stage for SSH debugging
FROM debugger AS ssh-debugger
USER root
# Install OpenSSH server and rsync
# Clean the yum cache and remove unnecessary packages
# Setup SSH for root access
RUN --mount=target=/var/lib/apt/lists,type=cache,sharing=locked \
    --mount=target=/var/cache/apt,type=cache,sharing=locked \
    rm -f /etc/apt/apt.conf.d/docker-clean && \
    apt update && \
    apt install -y --no-install-recommends openssh-server rsync && \
    mkdir /var/run/sshd && \
    ssh-keygen -A && \
    echo 'debugger:debugger' | chpasswd && \
    sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

USER debugger

# Expose port 22 for SSH access
EXPOSE 22

# Start the SSH daemon
CMD ["/usr/sbin/sshd","-D"]

FROM builder AS binary

# Build OSU Benchmarks
ARG OSU_VERSION="7.2"
ARG OSU_CONFIGURE_OPTIONS="--prefix=/usr/local CC=mpicc CXX=mpicxx CFLAGS=-O3 --enable-cuda --with-cuda=/usr/local/cuda"

RUN mkdir -p /tmp/osu-benchmark-build \
      && cd /tmp/osu-benchmark-build \
      && wget http://mvapich.cse.ohio-state.edu/download/mvapich/osu-micro-benchmarks-${OSU_VERSION}.tar.gz \
      && tar xzvf osu-micro-benchmarks-${OSU_VERSION}.tar.gz \
      && cd osu-micro-benchmarks-${OSU_VERSION} \
      && ./configure ${OSU_CONFIGURE_OPTIONS} \
      && make -j$(nproc) \
      && make install \
      && cd / \
      && rm -rf /tmp/osu-benchmark-build
ENV PATH="/usr/local/libexec/osu-micro-benchmarks/mpi/collective:/usr/local/libexec/osu-micro-benchmarks/mpi/one-sided:/usr/local/libexec/osu-micro-benchmarks/mpi/pt2pt:/usr/local/libexec/osu-micro-benchmarks/mpi/startup:$PATH"

WORKDIR /
CMD ["/bin/bash"]
