import gmsh
import sys
import numpy
import meshio

from dolfin import MPI, MeshValueCollection, cpp

# Map from gmsh to type that is also used by meshio
mapping = {
    1: "line",
    2: "triangle",
    4: "tetra",
    8: "line3",
    9: "triangle6",
    11: "tetra10",
    15: "vertex",
}
nodes = {
    "line": 2,
    "triangle": 3,
    "tetra": 4,
    "line3": 3,
    "triangle6": 6,
    "tetra10": 10,
    "vertex": 1,
}

# Generate mesh
mesh_ele_size = 0.5

gmsh.initialize(sys.argv)
gmsh.model.add("unit_square")
gmsh.model.geo.addPoint(0.0, 0.0, 0.0, mesh_ele_size, 1)
gmsh.model.geo.addPoint(1.0, 0.0, 0.0, mesh_ele_size, 2)
gmsh.model.geo.addPoint(1.0, 1.0, 0.0, mesh_ele_size, 3)
gmsh.model.geo.addPoint(0.0, 1.0, 0.0, mesh_ele_size, 4)

gmsh.model.geo.addLine(1, 2, 1)
gmsh.model.geo.addLine(2, 3, 2)
gmsh.model.geo.addLine(3, 4, 3)
gmsh.model.geo.addLine(4, 1, 4)

gmsh.model.geo.addCurveLoop([1, 2, 3, 4], 1)
gmsh.model.geo.addPlaneSurface([1], 6)

gmsh.model.addPhysicalGroup(1, [1], 1)
gmsh.model.setPhysicalName(1, 1, "BOTTOM")

gmsh.model.addPhysicalGroup(1, [3], 2)
gmsh.model.setPhysicalName(1, 2, "TOP")

gmsh.model.addPhysicalGroup(1, [4], 3)
gmsh.model.setPhysicalName(1, 3, "LEFT")

gmsh.model.addPhysicalGroup(1, [2], 4)
gmsh.model.setPhysicalName(1, 4, "RIGHT")

gmsh.model.addPhysicalGroup(2, [6], 6)
gmsh.model.setPhysicalName(2, 6, "DOMAIN")

gmsh.model.geo.synchronize()
gmsh.model.mesh.generate()

nodeTags, coord, parametricCoord = gmsh.model.mesh.getNodes()
dim = 3

# Reshape to get right format
points = numpy.reshape(coord, (int(coord.size / dim), dim))
element_types, element_tags, node_tags = gmsh.model.mesh.getElements()

cells = {}
cell_data = {}

# Generate cells and cell_data dict
for num, element in enumerate(element_types):
    name = mapping[element]
    num_nodes = nodes[name]
    # since nodes are numbered starting from 0
    cells[name] = node_tags[num] - 1
    cells[name] = numpy.reshape(
        cells[name], (numpy.int(cells[name].size / num_nodes), num_nodes)
    )
    # If mesh contains the physical group of dimension dim
    # -1 to match with gmsh api dimensions
    dim_tags = gmsh.model.getPhysicalGroups(num_nodes - 1)
    if dim_tags:
        for dim_tag in dim_tags:
            dim = dim_tag[0]
            tag = dim_tag[1]
            element_type, element_tag, node_tag = gmsh.model.mesh.getElements(dim, tag)
            for ele_num in element_tag[0]:
                element_tags[dim - 1][element_tags[dim - 1] == ele_num] = tag
        cell_data[name] = element_tags[dim - 1]

# gmsh.write("unit_square.geo_unrolled")

# End Generate mesh
gmsh.finalize()

meshio.write(
    "unit_square_gmsh.xdmf",
    meshio.Mesh(points=points, cells={"triangle": cells["triangle"]}),
)

mesh = cpp.mesh.Mesh(
    MPI.comm_world,
    cpp.mesh.CellType.triangle,
    points,
    cells["triangle"],
    [],
    cpp.mesh.GhostMode.none,
)

mvc_from_array = MeshValueCollection(
    "size_t", mesh, 1, cells["line"], cell_data["line"]
)

if MPI.comm_world.rank == 0:
    print(mvc_from_array.values())
