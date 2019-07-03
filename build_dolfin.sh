#! /bin/bash

cd Submodules/dolfinx/cpp/build && \
	make install -j 2

cd ../../python

pip3 install -e . --upgrade