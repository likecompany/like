FROM archlinux:latest

WORKDIR /app/build

COPY . /app

RUN pacman -Syy && \
    pacman -S --noconfirm git \
      gcc \
      cmake make \
      boost

RUN git clone https://github.com/redis/hiredis.git && \
    git clone https://github.com/sewenew/redis-plus-plus.git && \
    git clone https://github.com/nodejs/http-parser

WORKDIR /app/build/hiredis

RUN make && make install

WORKDIR /app/build/redis-plus-plus/compile

RUN cmake .. && make && make install

WORKDIR /app/build/http-parser

RUN make && make install

WORKDIR /app/build

RUN cmake .. && make
