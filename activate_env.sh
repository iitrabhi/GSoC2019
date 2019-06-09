#! /bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

export PYTHONPATH=${PYTHONPATH}:${DIR}/submodules/dijitso
export PYTHONPATH=${PYTHONPATH}:${DIR}/submodules/dolfinx/python
export PYTHONPATH=${PYTHONPATH}:${DIR}/submodules/ffcx
export PYTHONPATH=${PYTHONPATH}:${DIR}/submodules/fiat
export PYTHONPATH=${PYTHONPATH}:${DIR}/submodules/minidolfin
export PYTHONPATH=${PYTHONPATH}:${DIR}/submodules/ufl
export PYTHONPATH=${PYTHONPATH}:${DIR}/simd/ttbench
