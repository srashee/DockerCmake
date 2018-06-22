# Dockerfile for Select Server project.
# This dockerfile uses Ubuntu as the base image
# to run the server.
# Since Windows is able to run linux containers and
# The opposite is not true, we are only creating a
# Linux container for use by both Linux and Windows users.
FROM ubuntu:16.04

ENV PATH /pp/bin:$PATH

WORKDIR /app

# Creating two users. One user will be used to build the Server
# The other user will be used by consumers of the Server container
# All environment dependencies should be installed here

RUN export DEBIAN_FRONTEND=noninteractive && \
    useradd --system \
      --create-home \
      --shell /usr/sbin/nologin \
      SS-build && \
    useradd --system \
      --create-home \
      --shell /usr/sbin/nologin \
      SS && \
      apt-get update && \
      apt-get -y install make && \
      apt-get -y install gcc && \
      rm -rf /var/lib/apt/lists/*

# Copy source
COPY . /tmp/build/

RUN mkdir -p /app && \
	chown -R SS-build:SS-build /tmp/build /app

USER SS-build

RUN cd /tmp/build && \
	make && \
	cp SelectServer /app && \
    cd /app && \
	rm -rf /tmp/*

USER SS

CMD ./SelectServer
