FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    mesa-utils \
    cmake \
    libsdl2-dev \
    libeigen3-dev \
    libglew-dev \
    libopengl-dev \
    libglvnd-dev \
    build-essential
    # Add other necessary libraries for your application

# Build command: docker build -t test_opengl_v1 -f Dockerfile_opengl .
# Run container: docker run -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -it -v "$PWD":/data test_opengl_v1