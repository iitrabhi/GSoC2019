#! /bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
# print the contents of the variable on screen
echo ${DIR}
docker run -v ${DIR}:/root/ -it quay.io/fenicsproject/dolfinx:latest
