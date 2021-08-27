FROM centos:8

RUN dnf update -y

RUN dnf install -y epel-release && \
    dnf install -y ragel make wget python38 rpm-build && \
    dnf install -y qt5-qtbase-devel openssl-devel boost-devel zlib-devel && \
    dnf install -y gcc-toolset-9-gcc gcc-toolset-9-gcc-c++ && \
    dnf --enablerepo=powertools install -y ninja-build

RUN wget https://github.com/Kitware/CMake/releases/download/v3.20.2/cmake-3.20.2-linux-x86_64.sh && \
    chmod 755 cmake-3.20.2-linux-x86_64.sh && \
    ./cmake-3.20.2-linux-x86_64.sh --prefix=/opt/ --exclude-subdir --skip-license

ENV PATH=/opt/rh/gcc-toolset-9/root/usr/bin:/opt/bin:$PATH
