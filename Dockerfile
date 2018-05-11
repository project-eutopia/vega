FROM alpine
MAINTAINER Christopher Locke <project.eutopia@gmail.com>

RUN apk update && apk upgrade && \
    apk add --no-cache git \
                       make \
                       cmake \
                       g++ \
                       zlib-dev \
                       gtest \
                       gtest-dev \
                       bash

RUN chmod +x /usr/lib/libgtest*.so

ENV VEGA_DIR "/opt/vega"
RUN mkdir -p $VEGA_DIR
WORKDIR $VEGA_DIR

# Add current folder, and remove all extraneous working files
ADD . $VEGA_DIR
RUN git reset --hard HEAD
RUN git clean -fd

# Build the main exectable and tests
RUN mkdir -p build && \
    cd build && \
    rm -rf ./* && \
    cmake .. && \
    make -j8 main tests && \
    cd ..
