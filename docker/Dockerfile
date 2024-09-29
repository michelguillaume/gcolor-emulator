FROM ubuntu:24.10

# Enable Universe repositories and install necessary tools (including g++-14)
RUN apt-get update && apt-get install -y software-properties-common \
    && add-apt-repository universe \
    && apt-get update \
    && apt-get install -y \
    gcc-14 g++-14 \
    make \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*

ENV CC=gcc-14
ENV CXX=g++-14
