import pygmsh
from dolfin import MPI, MeshValueCollection, cpp

geom = pygmsh.opencascade.Geometry()

mesh_ele_size = 0.5
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

geom.set_transfinite_surface(ps, size=[4, 4])
box = geom.extrude(ps, [0, 0, 1], num_layers=4)

# Tag line and surface
geom.add_physical([p0, p3], label="POINT_LEFT")
geom.add_physical([p1, p2], label="POINT_RIGHT")
geom.add_physical([l0, l2], label="LINE_X")
geom.add_physical([l1, l3], label="LINE_Y")
geom.add_physical(ps, label="SURFACE")
geom.add_physical(box[1], label="BOX")

pygmsh_mesh = pygmsh.generate_mesh(geom,geo_filename="tetra.geo")
points, cells, cell_data = pygmsh_mesh.points, pygmsh_mesh.cells, pygmsh_mesh.cell_data

mesh = cpp.mesh.Mesh(MPI.comm_world,
                     cpp.mesh.CellType.tetrahedron, points,
                     cells['tetra'], [], cpp.mesh.GhostMode.none)
assert mesh.degree() == 1
assert mesh.geometry.dim == 3
assert mesh.topology.dim == 3

mvc_vertex = MeshValueCollection("size_t", mesh, 0, cells["vertex"], cell_data["vertex"]['gmsh:physical'])
assert mvc_vertex.get_value(0, 0) == 1

mvc_line = MeshValueCollection("size_t", mesh, 1, cells["line"], cell_data["line"]['gmsh:physical'])
assert mvc_line.get_value(0, 4) == 4

mvc_triangle = MeshValueCollection("size_t", mesh, 2, cells["triangle"], cell_data["triangle"]['gmsh:physical'])
assert mvc_triangle.get_value(0, 3) == 5

mvc_tetra = MeshValueCollection("size_t", mesh, 3, cells["tetra"], cell_data["tetra"]['gmsh:physical'])
assert mvc_tetra.get_value(0, 0) == 6
pass
