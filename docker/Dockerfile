FROM ubuntu:latest

WORKDIR /app/build

RUN apt update && \
    apt install -y \
    git \
    gcc g++ \
    libstdc++6 \
    build-essential cmake make \
    libboost-all-dev

RUN git clone https://github.com/redis/hiredis.git
WORKDIR /app/build/hiredis
RUN make
RUN make install

WORKDIR /app/build
RUN git clone https://github.com/sewenew/redis-plus-plus.git
WORKDIR /app/build/redis-plus-plus
RUN mkdir compile
WORKDIR /app/build/redis-plus-plus/compile
RUN cmake  ..
RUN make
RUN make install

WORKDIR /app/build
RUN git clone https://github.com/nodejs/http-parser
WORKDIR /app/build/http-parser
RUN make
RUN make install

WORKDIR /app/build
