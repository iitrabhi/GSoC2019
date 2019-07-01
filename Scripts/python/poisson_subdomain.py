from pygmsh.built_in.geometry import Geometry
from pygmsh import generate_mesh
import meshio
import dolfin
import dolfin.io

geom = Geometry()

mesh_ele_size = .2
p1 = geom.add_point([0.0, 0.0, 0], lcar=mesh_ele_size)
p2 = geom.add_point([0.0, 1.0, 0], lcar=mesh_ele_size)
p3 = geom.add_point([1.0, 1.0, 0], lcar=mesh_ele_size)
p4 = geom.add_point([1.0, 0.0, 0], lcar=mesh_ele_size)
p5 = geom.add_point([0.2, 0.5, 0], lcar=mesh_ele_size)
p6 = geom.add_point([0.2, 0.7, 0], lcar=mesh_ele_size)
p7 = geom.add_point([1.0, 0.5, 0], lcar=mesh_ele_size)
p8 = geom.add_point([1.0, 0.7, 0], lcar=mesh_ele_size)

l1 = geom.add_line(p1, p4)
l2 = geom.add_line(p3, p2)
l3 = geom.add_line(p2, p1)
l4 = geom.add_line(p7, p5)
l5 = geom.add_line(p5, p6)
l6 = geom.add_line(p6, p8)
l7 = geom.add_line(p4, p7)
l8 = geom.add_line(p7, p8)
l9 = geom.add_line(p8, p3)

ll1 = geom.add_line_loop(lines=[l2, l3, l1, l7, l4, l5, l6, l9])
ps1 = geom.add_plane_surface(ll1)

ll2 = geom.add_line_loop(lines=[l6, -l8, l4, l5])
ps2 = geom.add_plane_surface(ll2)

# Tag line and surface
geom.add_physical(l3, label="LEFT")
geom.add_physical(l2, label="TOP")
geom.add_physical([l9, l8, l7], label="RIGHT")
geom.add_physical(l1, label="BOTTOM")


geom.add_physical(ps1, label="DOMAIN")
geom.add_physical(ps2, label="OBSTACLE")

print("\n".join(geom._GMSH_CODE))

mesh = generate_mesh(geom)
points, cells, cell_data = mesh.points, mesh.cells, mesh.cell_data

mesh = dolfin.cpp.mesh.Mesh(
    dolfin.MPI.comm_world, 
    dolfin.cpp.mesh.CellType.Type.triangle, 
    points,
    cells['triangle'], 
    [], 
    dolfin.cpp.mesh.GhostMode.none)

mvc_subdomain = dolfin.MeshValueCollection("size_t", 
    mesh,
    2, 
    cells["triangle"].tolist(), 
    cell_data["triangle"]['gmsh:physical'].tolist())


mvc_boundaries = dolfin.MeshValueCollection("size_t", 
	mesh,
	1, 
	cells["line"].tolist(), 
	cell_data["line"]['gmsh:physical'].tolist())

print("Constructing MeshFunction from MeshValueCollection")
domains = dolfin.cpp.mesh.MeshFunctionSizet(mesh, mvc_subdomain, 0)
boundaries = dolfin.cpp.mesh.MeshFunctionSizet(mesh, mvc_boundaries, 0)

pass
