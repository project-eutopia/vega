FROM alpine
MAINTAINER Christopher Locke <project.eutopia@gmail.com>

RUN apk update && apk upgrade && \
    apk add --no-cache git \
                       make \
                       cmake \
                       clang \
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

ENV CC "/usr/bin/gcc"
ENV CXX "/usr/bin/g++"

RUN mkdir -p build_gcc && \
    cd build_gcc && \
    rm -rf ./* && \
    cmake -DCMAKE_BUILD_TYPE=DEBUG .. && \
    make -j8 main tests && \
    cd ..

ENV CC "/usr/bin/clang"
ENV CXX "/usr/bin/clang++"

RUN mkdir -p build_clang && \
    cd build_clang && \
    rm -rf ./* && \
    cmake -DCMAKE_BUILD_TYPE=DEBUG .. && \
    make -j8 main tests && \
    cd ..
