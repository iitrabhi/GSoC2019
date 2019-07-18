import gmsh
import sys
import numpy

# Map from gmsh to type that is also used by meshio
mapping = {1: 'line',
           2: 'triangle',
           4: 'tetra',
           8: 'line3',
           9: 'triangle6',
           11: 'tetra10',
           15: 'vertex'
           }
nodes = {'line': 2,
         'triangle': 3,
         'tetra': 4,
         'line3': 3,
         'triangle6': 6,
         'tetra10': 10,
         'vertex': 1
         }

# Generate mesh
gmsh.initialize(sys.argv)
gmsh.model.add('ball')
gmsh.model.occ.addSphere(0.0, 0.0, 0.0, 1.0)
gmsh.model.occ.synchronize()
gmsh.model.mesh.generate()
nodeTags, coord, parametricCoord = gmsh.model.mesh.getNodes()
dim = 3
# Reshape to get right format
points = numpy.reshape(coord, (numpy.int(coord.size / dim), dim))
elementTypes, elementTags, nodeTags = gmsh.model.mesh.getElements()
cells = {}
# Generate cells dict
for num, element in enumerate(elementTypes):
    name = mapping[element]
    dim = nodes[name]
    cells[name] = nodeTags[num] - 1  # since nodes are numbered starting from 0
    cells[name] = numpy.reshape(cells[name], (numpy.int(cells[name].size / dim), dim))
# End Generate mesh
gmsh.finalize()

