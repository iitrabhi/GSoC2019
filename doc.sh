#! /bin/bash
cd Submodules/dolfinx/python/demo/ && python3 ./generate-demo-files.py
cd ../doc && make html