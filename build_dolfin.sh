#! /bin/bash

cd Submodules/dolfinx/cpp/build && \
	make install

cd ../../python

pip3 install -e .