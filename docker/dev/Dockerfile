FROM ubuntu:20.04 as hakoniwa-suzmosim-dev

ARG DEBIAN_FRONTEND=noninteractive

ENV CMAKE_INSTALL_DIR /usr/local/cmake

RUN apt-get update
RUN apt-get install -y \
	git	\
	build-essential	\
	wget	\
	gcc	\
	g++	\
	vim	\
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


RUN	wget -q -O cmake-linux.sh https://github.com/Kitware/CMake/releases/download/v3.17.0/cmake-3.17.0-Linux-x86_64.sh && \
	mkdir -p $CMAKE_INSTALL_DIR && \
	sh cmake-linux.sh --skip-license --prefix=$CMAKE_INSTALL_DIR && \
	rm cmake-linux.sh
ENV PATH ${PATH}:${CMAKE_INSTALL_DIR}/bin


RUN apt-get update
RUN apt-get install -y libc++-dev libc++abi-dev
RUN apt-get install -y python3-pip  libffi-dev python3-dev
RUN pip3 install numpy
RUN pip3 install pyquaternion
RUN pip3 install scipy

RUN mkdir /usr/local/lib/hakoniwa
RUN mkdir /usr/local/bin/hakoniwa

ENV PYTHONPATH="/usr/local/lib/hakoniwa:$PYTHONPATH"
RUN apt-get update
WORKDIR /root
RUN git clone --recursive https://github.com/toppers/hakoniwa-core-cpp-client.git
WORKDIR /root/hakoniwa-core-cpp-client
RUN apt-get install -y python3-pip  libffi-dev python3-dev
RUN bash build.bash
RUN cp cmake-build/src/hakoc/libshakoc.* /usr/local/lib/hakoniwa/
RUN cp cmake-build/src/hakoc/libhakoarun.* /usr/local/lib/hakoniwa/
RUN cp cmake-build/core/src/libhako.* /usr/local/lib/hakoniwa/
RUN cp cmake-build/src/hakoc/libshakoc.so /usr/local/lib/hakoniwa/hakoc.so
RUN cp -rp py /usr/local/lib/hakoniwa/
ENV PYTHONPATH="/usr/local/lib/hakoniwa/py:$PYTHONPATH"
RUN cp cmake-build/src/proxy/hako-proxy /usr/local/bin/hakoniwa
RUN cp cmake-build/core/sample/base-procs/hako-cmd/hako-cmd /usr/local/bin/hakoniwa

RUN mkdir /root/workspace
WORKDIR /root/workspace

CMD [ "/root/workspace/build.bash" ]
