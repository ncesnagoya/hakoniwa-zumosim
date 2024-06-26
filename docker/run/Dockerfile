FROM ubuntu:20.04 as hakoniwa-zumosim-run

ARG DEBIAN_FRONTEND=noninteractive

ENV CMAKE_INSTALL_DIR /usr/local/cmake
ENV PATH $PATH:/usr/local/bin/hakoniwa
ENV LD_LIBRARY_PATH $LD_LIBRARY_PATH:/usr/local/lib/hakoniwa

RUN apt-get update && apt-get install -y \
	git	\
	build-essential	\
	wget	\
	gcc	\
	g++	\
	build-essential	\
	vim	\
	protobuf-compiler \
	libssl-dev libreadline-dev zlib1g-dev \
	make	\
	autoconf \
	automake \
	pkg-config \
	curl \
	net-tools \
	netcat \
	&& apt-get clean \
	&& rm -rf /var/lib/apt/lists/*


RUN mkdir /root/components
WORKDIR /root/components

RUN	wget -q -O cmake-linux.sh https://github.com/Kitware/CMake/releases/download/v3.17.0/cmake-3.17.0-Linux-x86_64.sh && \
	mkdir -p $CMAKE_INSTALL_DIR && \
	sh cmake-linux.sh --skip-license --prefix=$CMAKE_INSTALL_DIR && \
	rm cmake-linux.sh
ENV PATH ${PATH}:${CMAKE_INSTALL_DIR}/bin

RUN mkdir /usr/local/lib/hakoniwa
RUN mkdir /usr/local/bin/hakoniwa

ENV RUST_HOME /usr/local/lib/rust
ENV RUSTUP_HOME ${RUST_HOME}/rustup
ENV CARGO_HOME ${RUST_HOME}/cargo
RUN mkdir /usr/local/lib/rust && \
    chmod 0755 $RUST_HOME
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs > ${RUST_HOME}/rustup.sh \
    && chmod +x ${RUST_HOME}/rustup.sh \
    && ${RUST_HOME}/rustup.sh -y --default-toolchain nightly --no-modify-path
ENV PATH $PATH:$CARGO_HOME/bin

RUN apt-get update
RUN apt-get install -y libc++-dev libc++abi-dev
RUN apt-get install -y python3-pip  libffi-dev python3-dev
RUN pip3 install numpy
RUN pip3 install pyquaternion
RUN pip3 install scipy

ENV PYTHONPATH="/usr/local/lib/hakoniwa:$PYTHONPATH"
WORKDIR /root
RUN git clone -b v1.0.4 --recursive https://github.com/toppers/hakoniwa-core-cpp-client.git
WORKDIR /root/hakoniwa-core-cpp-client
RUN apt-get install -y python3-pip  libffi-dev python3-dev
RUN bash build.bash
RUN cp cmake-build/src/hakoc/libshakoc.* /usr/local/lib/hakoniwa/
RUN cp cmake-build/src/hakoc/libshakoc.so /usr/local/lib/hakoniwa/hakoc.so
RUN cp -rp py /usr/local/lib/hakoniwa/
ENV PYTHONPATH="/usr/local/lib/hakoniwa/py:$PYTHONPATH"
RUN cp cmake-build/src/proxy/hako-proxy /usr/local/bin/hakoniwa
RUN cp cmake-build/core/sample/base-procs/hako-cmd/hako-cmd /usr/local/bin/hakoniwa

WORKDIR /root
RUN git clone --recursive https://github.com/toppers/hakoniwa-conductor.git
WORKDIR /root/hakoniwa-conductor/main
RUN bash install.bash
RUN cp /usr/local/bin/hakoniwa/hako-master  /usr/local/bin/hakoniwa/hakoniwa-conductor 
WORKDIR /root/workspace

CMD [ "/root/workspace/run.bash" ]
