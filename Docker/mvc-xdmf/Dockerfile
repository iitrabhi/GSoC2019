ARG TINI_VERSION=v0.18.0

FROM quay.io/fenicsproject/dolfinx:latest as base

WORKDIR /tmp

# For validating XDMF using xmllint
RUN apt-get update && apt-get upgrade -y && apt-get install -y libxml2-utils

# Install black for checking python code
RUN pip3 install black

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

########################################

FROM base as notebook
LABEL description="DOLFIN-X Jupyter Notebook"
WORKDIR /root

ARG TINI_VERSION
ADD https://github.com/krallin/tini/releases/download/${TINI_VERSION}/tini /tini
RUN chmod +x /tini && \
    pip3 install --no-cache-dir jupyter jupyterlab

ENTRYPOINT ["/tini", "--", "jupyter", "notebook", "--ip", "0.0.0.0", "--no-browser", "--allow-root"]

########################################