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
geom.add_physical(l3, label="LINE")
geom.add_physical(ps, label="SURFACE")

print("\n".join(geom._GMSH_CODE))

mesh = generate_mesh(geom)
points, cells, cell_data, boundary = mesh.points, mesh.cells, mesh.cell_data, mesh.field_data

mesh_from_array = dolfin.cpp.mesh.Mesh(
    dolfin.MPI.comm_world, 
    dolfin.cpp.mesh.CellType.Type.triangle, 
    points,
    cells['triangle'], 
    [], 
    dolfin.cpp.mesh.GhostMode.none)

mvc_from_array = dolfin.MeshValueCollection("size_t", 
	mesh_from_array,
	1, 
	cells["line"].tolist(), 
	cell_data["line"]['gmsh:physical'].tolist())

print("Constructing MeshFunction from MeshValueCollection")
mf = dolfin.cpp.mesh.MeshFunctionSizet(mesh_from_array, mvc_from_array, 1)

# ToDo: Convert field data to String:Int
# or make read and write info more general.
boundary['LINE'] = 1
boundary['SURFACE'] = 2

file = dolfin.io.XDMFFile(dolfin.MPI.comm_world, "input/mesh_from_dolfin.xdmf")
file.write(mesh_from_array)
file.write(boundary)
pass
