import meshio
from dolfin import MPI, cpp
from dolfin.io import XDMFFile
from pygmsh import generate_mesh
from pygmsh.built_in.geometry import Geometry

# -----------------Step - 1 - Define mesh --------------
geom = Geometry()

mesh_ele_size = 0.2
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

# print("\n".join(geom._GMSH_CODE))

msh = generate_mesh(geom)
points, cells = msh.points, msh.cells
cell_data, field_data = msh.cell_data, msh.field_data

# -----------------Step - 2 - Convert mesh --------------
meshio.write("input/poisson_subdomain.xdmf", meshio.Mesh(
    points=points,
    cells={"triangle": cells["triangle"]},
    field_data=field_data))

# -----------------Step - 3 - Read mesh -----------------
with XDMFFile(MPI.comm_world,
              "input/poisson_subdomain.xdmf") as xdmf_infile:
    mesh = xdmf_infile.read_mesh(cpp.mesh.GhostMode.none)
    tags = xdmf_infile.read_information_int()
    print(tags)
# -----------------Step - 3 - Read MVC -----------------

pass
