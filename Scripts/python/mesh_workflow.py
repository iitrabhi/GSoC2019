import dolfin
import dolfin.io

with dolfin.io.XDMFFile(dolfin.MPI.comm_world, "input/unit_square.xdmf") as xdmf_infile:
    mesh_2d = xdmf_infile.read_mesh(dolfin.cpp.mesh.GhostMode.none)

# mvc = dolfin.MeshValueCollection("size_t", mesh_2d, 1)
# 
# with dolfin.io.XDMFFile(dolfin.MPI.comm_world, "input/mvc_1d.xdmf") as xdmf_infile:
#     mvc = xdmf_infile.read_mvc_size_t(mesh_2d, "name_to_read")
# 
# print("Constructing MeshFunction from MeshValueCollection")
# mf = dolfin.cpp.mesh.MeshFunctionSizet(mesh_2d, mvc, 1)

pass
