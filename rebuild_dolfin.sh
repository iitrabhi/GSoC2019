#! /bin/bash

rm -rf Submodules/dolfinx/cpp/build
rm -rf Submodules/dolfinx/python/build
rm -rf Submodules/dolfinx/python/fenics_dolfin.egg-info

cd  Submodules/dolfinx/cpp && \
	mkdir build && \
	cd build && \
	cmake  .. && \
	make install

cd ../../python

pip3 install -e .
