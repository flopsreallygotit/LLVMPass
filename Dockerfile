FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    wget gnupg cmake ninja-build git \
    libgraphviz-dev graphviz \
    && rm -rf /var/lib/apt/lists/*

RUN wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key \
        > /etc/apt/trusted.gpg.d/apt.llvm.org.asc && \
    echo "deb http://apt.llvm.org/noble/ llvm-toolchain-noble-19 main" \
        > /etc/apt/sources.list.d/llvm.list && \
    apt-get update && apt-get install -y \
        clang-19 \
        llvm-19-dev \
    && rm -rf /var/lib/apt/lists/*

RUN ln -s /usr/bin/clang-19    /usr/bin/clang && \
    ln -s /usr/bin/clang++-19  /usr/bin/clang++ && \
    ln -s /usr/bin/opt-19      /usr/bin/opt && \
    ln -s /usr/bin/llc-19      /usr/bin/llc

WORKDIR /project

COPY . .

RUN rm -rf build

RUN cmake -G Ninja \
    -DCMAKE_C_COMPILER=clang-19 \
    -DCMAKE_CXX_COMPILER=clang++-19 \
    -DLLVM_DIR=/usr/lib/llvm-19/lib/cmake/llvm \
    -B build \
    && cmake --build build
