import pygmsh
from dolfin import (MPI, Cells, FacetRange, MeshFunction, MeshValueCollection,
                    UnitSquareMesh, VertexRange, cpp)

geom = pygmsh.opencascade.Geometry()

mesh_ele_size = .5
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

pygmsh_mesh = pygmsh.generate_mesh(geom)
points, cells, cell_data = pygmsh_mesh.points, pygmsh_mesh.cells, pygmsh_mesh.cell_data

mesh = cpp.mesh.Mesh(MPI.comm_world,
                     cpp.mesh.CellType.Type.triangle, points,
                     cells['triangle'], [], cpp.mesh.GhostMode.none)
assert mesh.degree() == 1
assert mesh.geometry.dim == 3
assert mesh.topology.dim == 2

f = MeshValueCollection("size_t", mesh, 1, cells["line"], cell_data["line"]['gmsh:physical'])
print(f.values())