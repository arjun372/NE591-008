# This Dockerfile creates a multi-stage build with different stages for building, debugging, and running the binary.
# It uses CentOS 8 as the base image and installs various development tools and libraries.
# It also sets up SSH for root access in the ssh-debugger stage.
# The purpose of this Dockerfile is to mimic the OS and packages available on the 'remote.eos.ncsu.edu' servers.

# Use the official CentOS 8 image as the base image
FROM centos:8 as base

# Update the CentOS repositories to use the vault repositories instead of the mirror ones
# Install the "Development Tools" group of packages, EPEL repository, Boost libraries and CMake
# Clean the yum cache and remove unnecessary packages
RUN cd /etc/yum.repos.d/ && \
    sed -i 's/mirrorlist/#mirrorlist/g' /etc/yum.repos.d/CentOS-* && \
    sed -i 's|#baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g' /etc/yum.repos.d/CentOS-* && \
    yum -y update && \
    yum -y groupinstall "Development Tools" && \
    yum -y install epel-release boost-devel cmake openmpi && yum -y clean all && \
    yum -y autoremove && dnf clean all && \
    rm -rf /var/cache/*

# Create a new stage based on the base stage
FROM base as builder

# Build type
ARG CMAKE_BUILD_TYPE=Release
# Build the tests
ARG CMAKE_BUILD_TESTS=ON
# Build the tests
ARG CMAKE_BUILD_PERFORMANCE_BENCHMARKS=ON
# Optimize for native builds, trading portability for performance
ARG CMAKE_PORTABLE=OFF

# Set the environment variables for the C and C++ compilers
ENV CC=/usr/bin/gcc
ENV CXX=/usr/bin/g++

# Set the working directory to /usr/src
WORKDIR /usr/src

# Copy the current directory contents into the container at /usr/src
COPY . .

# Build the project using CMake and make in the build folder
WORKDIR /usr/src/build
RUN cmake .. \
    -D CMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
    -D BUILD_TESTS=$CMAKE_BUILD_TESTS \
    -D BUILD_PERFORMANCE_BENCHMARKS=$CMAKE_BUILD_PERFORMANCE_BENCHMARKS \
    -D PORTABLE=$CMAKE_PORTABLE \
    && make -j$(nproc)

# Create a new stage based on the base stage for debugging
FROM base as debugger

# Install gdb and gdb-gdbserver for debugging and perf, systemtap for profiling
# Clean the yum cache and remove unnecessary packages
RUN yum -y install gdb gdb-gdbserver perf systemtap strace python3 && \
    yum -y clean all && \
    yum -y autoremove && dnf clean all && \
    rm -rf /var/cache/*

# Create a new stage based on the debugger stage for SSH debugging
FROM debugger AS ssh-debugger

# Install OpenSSH server and rsync
# Clean the yum cache and remove unnecessary packages
# Setup SSH for root access
RUN yum -y install openssh-server rsync && \
    yum -y clean all && \
    yum -y autoremove && dnf clean all && \
    rm -rf /var/cache/* && \
    mkdir /var/run/sshd && \
    ssh-keygen -A && \
    echo 'root:debugger' | chpasswd && \
    echo 'PermitRootLogin yes' >> /etc/ssh/sshd_config && \
    sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

# Expose port 22 for SSH access
EXPOSE 22

# Start the SSH daemon
CMD ["/usr/sbin/sshd","-D"]

# Create a new stage based on the builder stage for running the binary
FROM builder as binary

# Set the working directory to /usr/src
WORKDIR /usr/src

# Start a bash shell
CMD ["/bin/bash"]
