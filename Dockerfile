# Use IMAGE to select the docker image to build from.
# For i386 builds:
# docker build --build-arg IMAGE=i386/ubuntu:18.04 -tmichaelfig/libsourcey:v1.1.4-i386 .
ARG IMAGE=ubuntu:18.04
FROM ${IMAGE}
MAINTAINER Kam Low <hello@sourcey.com>

# We need IMAGE again to influence the build.
ARG IMAGE=ubuntu:18.04

# Install dependencies
RUN apt-get update && apt-get install -y \
  build-essential \
  pkg-config \
  git \
  curl \
  cmake \
  libx11-dev \
  libglu1-mesa-dev \
  gcc g++ gdb \
  libssl-dev \
  libx264-dev yasm \
  libavcodec-dev libavdevice-dev libavfilter-dev libavformat-dev libswresample-dev libpostproc-dev

# Download and extract precompiled WebRTC static libraries
# COPY vendor/webrtc-22215-ab42706-linux-x64 /vendor/webrtc-22215-ab42706-linux-x64
# COPY *.tar.gz libsourcey/
RUN case "$IMAGE" in \
  *86*) dir=`{ cd libsourcey && ls -t webrtc-*-linux-x86.tar.gz | sed -e 's/\.tar\.gz$//; q'; } || echo webrtc-18252-6294a7e-linux-x86` ;; \
  *) dir=`{ cd libsourcey && ls -t webrtc-*-linux-x64.tar.gz | sed -e 's/\.tar\.gz$//; q'; } || echo webrtc-22215-ab42706-linux-x64` ;; \
  esac; \
  mkdir -p /vendor/$dir; \
  if test -f libsourcey/$dir.tar.gz; then cat libsourcey/$dir.tar.gz; \
  else curl -sSL https://github.com/sourcey/webrtc-precompiled-builds/raw/master/$dir.tar.gz; fi | tar -xzC /vendor/$dir

# Install LibSourcey
#COPY . libsourcey
RUN git clone https://github.com/sourcey/libsourcey.git && \
  dir=`ls -d /vendor/webrtc-*` && case "$dir" in *x86) export LDFLAGS=-m32; def=" -DCMAKE_CXX_FLAGS=-m32 -DCMAKE_C_FLAGS=-m32";; esac && echo "WebRTC in $dir$def" && \
  cd /libsourcey && mkdir build && cd build && \
  cmake -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_SHARED_LIBS=OFF -DBUILD_WITH_STATIC_CRT=ON \
           -DBUILD_MODULES=ON -DBUILD_APPLICATIONS=OFF -DBUILD_SAMPLES=OFF -DBUILD_TESTS=OFF \
           -DWITH_FFMPEG=ON -DWITH_WEBRTC=ON -DENABLE_LOGGING=ON \
           -DWEBRTC_ROOT_DIR="$dir"$def \
           -DCMAKE_INSTALL_PREFIX=/libsourcey/install .. && \
  make VERBOSE=1 && \
  make install
  # cachebust


# Set the working directory to the LibSourcey install directory
WORKDIR /libsourcey/install
