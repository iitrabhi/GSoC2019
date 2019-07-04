from pygmsh.built_in.geometry import Geometry
from pygmsh import generate_mesh
import meshio
import dolfin
import dolfin.io
from dolfin.plotting import plot
import matplotlib

# Define input data
from ufl import SpatialCoordinate, inner, grad, lhs, rhs, dot, exp, Measure, dx, ds
from dolfin.fem import assemble_scalar
from dolfin import FunctionSpace, TrialFunction, TestFunction, DirichletBC, Function, solve, cpp,fem    

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

#print("\n".join(geom._GMSH_CODE))

msh = generate_mesh(geom)
points, cells, cell_data, field_data = msh.points, msh.cells, msh.cell_data, msh.field_data

mesh = dolfin.cpp.mesh.Mesh(
    dolfin.MPI.comm_world, 
    dolfin.cpp.mesh.CellType.Type.triangle, 
    points[:,:2],# Converting to 2D
    cells['triangle'], 
    [], 
    dolfin.cpp.mesh.GhostMode.none)

mesh.geometry.coord_mapping = fem.create_coordinate_map(mesh)

mvc_boundaries = dolfin.MeshValueCollection("size_t", 
    mesh,
    1, 
    cells["line"].tolist(), 
    cell_data["line"]['gmsh:physical'].tolist())

mvc_subdomain = dolfin.MeshValueCollection("size_t", 
    mesh,
    2, 
    cells["triangle"].tolist(), 
    cell_data["triangle"]['gmsh:physical'].tolist())

print("Constructing MeshFunction from MeshValueCollection")
domains = dolfin.cpp.mesh.MeshFunctionSizet(mesh, mvc_subdomain, 0)
boundaries = dolfin.cpp.mesh.MeshFunctionSizet(mesh, mvc_boundaries, 0)

#print("Boundaries")
#print(mvc_boundaries.values())
#print(boundaries.array())
#print("Domains")
#print(mvc_subdomain.values())

#print(cells['line'])
#print(cell_data['line'])

#print(field_data)
#print(boundaries.array())
#print(boundaries.where_equal(1))
#print(boundaries.where_equal(2))
#print(boundaries.where_equal(3))
#print(boundaries.where_equal(4))

a0 = 1.0
a1 = 0.01
x = SpatialCoordinate(mesh)
g_L = exp(- 10*(- pow(x[1] - 0.5, 2)))
g_R = 1.0
f = 1.0

# Define function space and basis functions
V = FunctionSpace(mesh, ("CG", 2))
u = TrialFunction(V)
v = TestFunction(V)


u5 = Function(V)
with u5.vector().localForm() as bc_local:
    bc_local.set(5.0)

u0 = Function(V)
with u0.vector().localForm() as bc_local:
    bc_local.set(0.0)


# Define Dirichlet boundary conditions at top and bottom boundaries
bcs = [DirichletBC(V, u5, boundaries.where_equal(field_data['TOP'][0])),
       DirichletBC(V, u0, boundaries.where_equal(field_data['BOTTOM'][0]))]

dx = dx(subdomain_data=domains)
ds = ds(subdomain_data=boundaries)

# Define variational form
F = (inner(a0*grad(u), grad(v))*dx(field_data['DOMAIN'][0]) + 
    inner(a1*grad(u), grad(v))*dx(field_data['OBSTACLE'][0])
     - g_L*v*ds(field_data['LEFT'][0]) 
     - g_R*v*ds(field_data['RIGHT'][0])
     - f*v*dx(field_data['DOMAIN'][0]) 
     - f*v*dx(field_data['OBSTACLE'][0]))


# Separate left and right hand sides of equation
a, L = lhs(F), rhs(F)

# Solve problem
u = Function(V)
solve(a == L, u, bcs)


bb_tree = cpp.geometry.BoundingBoxTree(mesh, 2)
print(u([0.0, 1.0], bb_tree)[0])
#print((u.vector().array))

file = dolfin.io.XDMFFile(dolfin.MPI.comm_world, "input/saved_function.xdmf")
file.write(u)

pass
