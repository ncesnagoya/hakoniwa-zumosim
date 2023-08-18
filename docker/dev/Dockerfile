FROM ubuntu:20.04 as hakoniwa-suzmosim-dev

ARG DEBIAN_FRONTEND=noninteractive

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


RUN mkdir /root/workspace
WORKDIR /root/workspace

CMD [ "/root/workspace/build.bash" ]
