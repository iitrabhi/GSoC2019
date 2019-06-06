# https://github.com/ttroy50/pybind11-example
import pybindings
a = pybindings.XDMFFile("tag_all.xdmf")
a.read_mesh()