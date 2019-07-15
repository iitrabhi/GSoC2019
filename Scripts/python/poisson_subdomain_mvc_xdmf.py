import meshio
import dolfin
import dolfin.io
from dolfin.plotting import plot
import matplotlib

# Define input data
from ufl import SpatialCoordinate, inner, grad, lhs, rhs, dot, exp, Measure, dx, ds
from dolfin.fem import assemble_scalar
from dolfin import FunctionSpace, TrialFunction, TestFunction, DirichletBC, Function, solve, cpp,fem    

with dolfin.io.XDMFFile(dolfin.MPI.comm_world, "input/unit_square.xdmf") as xdmf_infile:
    mesh_2d = xdmf_infile.read_mesh(dolfin.MPI.comm_world, dolfin.cpp.mesh.GhostMode.none)
    boundary= xdmf_infile.read_information()
    print(boundary)

# mvc = dolfin.MeshValueCollection("size_t", mesh_2d, 1)
# 
# with dolfin.io.XDMFFile(dolfin.MPI.comm_world, "input/mvc_1d.xdmf") as xdmf_infile:
#     mvc = xdmf_infile.read_mvc_size_t(mesh_2d, "name_to_read")
# 
# print("Constructing MeshFunction from MeshValueCollection")
# mf = dolfin.cpp.mesh.MeshFunctionSizet(mesh_2d, mvc, 1)

# Step 1: Convert mesh to XDMF via meshio
points, cells, cell_data, boundary = msh.points, msh.cells, msh.cell_data, msh.field_data

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
bcs = [DirichletBC(V, u5, boundaries.where_equal(boundary['TOP'][0])),
       DirichletBC(V, u0, boundaries.where_equal(boundary['BOTTOM'][0]))]

dx = dx(subdomain_data=domains)
ds = ds(subdomain_data=boundaries)

# Define variational form
F = (inner(a0*grad(u), grad(v))*dx(boundary['DOMAIN'][0]) + 
    inner(a1*grad(u), grad(v))*dx(boundary['OBSTACLE'][0])
     - g_L*v*ds(boundary['LEFT'][0]) 
     - g_R*v*ds(boundary['RIGHT'][0])
     - f*v*dx(boundary['DOMAIN'][0]) 
     - f*v*dx(boundary['OBSTACLE'][0]))


# Separate left and right hand sides of equation
a, L = lhs(F), rhs(F)

# Solve problem
u = Function(V)
solve(a == L, u, bcs)


bb_tree = cpp.geometry.BoundingBoxTree(mesh, 2)
print(u([0.5, 0.5], bb_tree)[0])
#print((u.vector().array))

file = dolfin.io.XDMFFile(dolfin.MPI.comm_world, "input/saved_function.xdmf")
file.write(u)

pass
