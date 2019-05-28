from pygmsh.built_in.geometry import Geometry
from pygmsh import generate_mesh
import meshio
import dolfin
import dolfin.io

geom = Geometry()

mesh_ele_size = .1
p0 = geom.add_point([0, 0, 0], lcar=mesh_ele_size)
p1 = geom.add_point([1, 0, 0], lcar=mesh_ele_size)
p2 = geom.add_point([1, 1, 0], lcar=mesh_ele_size)
p3 = geom.add_point([0, 1, 0], lcar=mesh_ele_size)

l0 = geom.add_line(p0, p1)
l1 = geom.add_line(p1, p2)
l2 = geom.add_line(p2, p3)
l3 = geom.add_line(p3, p0)

ll = geom.add_line_loop(lines=[l0, l1, l2, l3])
ps = geom.add_plane_surface(ll)

# Tag line and surface
geom.add_physical(l3, label=444)
geom.add_physical(ps, label=456)

print("\n".join(geom._GMSH_CODE))

mesh = generate_mesh(geom)
points, cells, cell_data = mesh.points, mesh.cells, mesh.cell_data
meshio.write("output/mesh_2d.xdmf", meshio.Mesh(
    points=points,
    cells={"triangle": cells["triangle"]}))

meshio.write("output/mvc_1d.xdmf", meshio.Mesh(
    points=points,
    cells={"line": cells["line"]},
    cell_data={"line": {"name_to_read": cell_data["line"]["gmsh:physical"]}}
))

with dolfin.io.XDMFFile(dolfin.MPI.comm_world, "output/mesh_2d.xdmf") as xdmf_infile:
    mesh_2d = xdmf_infile.read_mesh(dolfin.MPI.comm_world, dolfin.cpp.mesh.GhostMode.none)

mvc = dolfin.MeshValueCollection("size_t", mesh_2d, 1)

with dolfin.io.XDMFFile(dolfin.MPI.comm_world, "output/mvc_1d.xdmf") as xdmf_infile:
    mvc = xdmf_infile.read_mvc_size_t(mesh_2d, "name_to_read")

print("Constructing MeshFunction from MeshValueCollection")
mf = dolfin.cpp.mesh.MeshFunctionSizet(mesh_2d, mvc, 1)

pass
