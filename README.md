# GSoC2019: GMsh/XDMF/DOLFIN mesh processing pipeline

This repository consist of code snippets and tests for the FEniCS-GSoC-2019 program. You can find the details about the project and my work plan [here](https://github.com/iitrabhi/GSoC2019/blob/master/PROPOSAL.md).

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. 

### Prerequisites

To follow along with the examples you need to install docker on your system.
* [Docker](https://www.docker.com/products/docker-desktop)

### Installing

Once the docker system in installed and running open terminal and run:

```
cd /path/to/this/repo
cd Docker/mvc-xdmf
docker build --target notebook -t mvc-xdmf-notebook .
```

```
cd /path/to/this/repo
cd Docker/mvc-xdmf
docker build --target base -t mvc-xdmf .
```

Note that you should have the file named Dockerfile in the directory where you run docker build.

## Running

After building the docker image you can start a Ipython notebook, just run:

```
docker run -p 8888:8888 mvc-xdmf-notebook:latest
``` 
To share share a host directory with the container run:

```
docker run -p 8888:8888 -v path/to/host/dir:/root/local mvc-xdmf-notebook:latest
```

## Authors

* [Abhinav Gupta](https://github.com/iitrabhi)
* [Michal Habera](https://github.com/michalhabera)
* [Jack S. Hale](https://github.com/jhale)
