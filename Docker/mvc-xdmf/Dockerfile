FROM quay.io/fenicsproject/dolfinx:latest

WORKDIR /tmp

# For validating XDMF using xmllint
RUN apt-get update && apt-get upgrade -y && apt-get install -y libxml2-utils

# docker build -t mvc-xdmf .
# Clone this repository

RUN git clone -b iitrabhi/mvc-xdmf https://github.com/iitrabhi/dolfinx.git && \
	cd dolfinx && \
	mkdir build && \
	cd build && \
	cmake -G Ninja ../cpp && \
	ninja ${MAKEFLAGS} install && \
	cd ../python && \
	pip3 install . --upgrade && \
	rm -rf /tmp/*

# Install meshio python package
RUN git clone -b iitrabhi/mvc-xdmf https://github.com/iitrabhi/meshio.git && \
	cd meshio && \
	pip3 install . --upgrade && \
	rm -rf /tmp/*

WORKDIR /root