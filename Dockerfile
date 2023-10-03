FROM ubuntu:18.04

RUN apt update && \
  apt -y install \
    cmake \
    g++-8 \
    gcc-8 \
    libasound2-dev \
    libglvnd-dev \
    libx11-dev \
    libxcursor-dev \
    libxi-dev \
    mesa-common-dev

ENV CXX=g++-8
ENV CC=gcc-8

COPY . /app

WORKDIR /app

RUN mkdir build && cd build && cmake .. && make -j
