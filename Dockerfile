# Below is our recipe for creating our docker container

FROM ubuntu:18.04
LABEL maintainer="Bryan Clark bryanclark86@gmail.com"
LABEL Description="Image for building and debugging arm-embedded projects from git"

# Install any needed packages specified in requirements.txt
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y \
    # Development files
      build-essential \
      git \
      bzip2 \
      wget && \
    apt-get clean
RUN cd /opt && \
    # Pick the correct compiler we want
    wget -qO- https://developer.arm.com/-/media/Files/downloads/gnu-rm/7-2017q4/gcc-arm-none-eabi-7-2017-q4-major-linux.tar.bz2 | tar -xj && \
    #wget -qO- https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-linux.tar.bz2 && \
    rm -f gcc-arm-linux.tar.bz2

ENV PATH "/opt/gcc-arm-eabi-7-2017-q4-major/bin:$PATH"
#ENV PATH "/opt/gcc-arm-eabi-9-2019-q4-major/bin:$PATH"

RUN cd / && \
    git config --global user.name "bryanclark90" && \
    git config --global user.email "bryanclark86@gmail.com" && \
    git clone https://github.com/crawfdan/stm.git

# now set up our work dir
WORKDIR /stm/build

ADD . /stm/build