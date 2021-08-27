FROM ubuntu:bionic

RUN apt-get update -y && \
    apt-get install software-properties-common -y && \
    add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
    apt-get install gcc-9 g++-9 \
                    qt5-default qtbase5-dev qt5-qmake qtbase5-dev-tools \
                    ragel ninja-build zlib1g-dev \
                    wget fuse -y && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 60 --slave /usr/bin/g++ g++ /usr/bin/g++-9

RUN wget https://github.com/Kitware/CMake/releases/download/v3.20.2/cmake-3.20.2-linux-x86_64.sh && \
    chmod 755 cmake-3.20.2-linux-x86_64.sh && \
    ./cmake-3.20.2-linux-x86_64.sh --prefix=/opt/ --exclude-subdir --skip-license

ENV PATH=/opt/bin:$PATH
