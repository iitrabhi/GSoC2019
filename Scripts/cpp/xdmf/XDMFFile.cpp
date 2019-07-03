#include <utility>

#include "XDMFFile.h"
#include "pugixml.hpp"
#include <iostream>
#include <assert.h>
#include <map>

#include "include/pybind11/pybind11.h"
#include "include/pybind11/stl.h"
#include "include/pybind11/stl_bind.h"

std::pair<std::string, int> get_cell_type(const pugi::xml_node& topology_node)
{
    assert(topology_node);
    pugi::xml_attribute type_attr = topology_node.attribute("TopologyType");
    assert(type_attr);

    const std::map<std::string, std::pair<std::string, int>> xdmf_to_dolfin
            = {{"polyvertex", {"point", 1}},
               {"Polyline", {"interval", 1}},
               {"edge_3", {"interval", 2}},
               {"triangle", {"triangle", 1}},
               {"triangle_6", {"triangle", 2}},
               {"tetrahedron", {"tetrahedron", 1}},
               {"tetrahedron_10", {"tetrahedron", 2}},
               {"quadrilateral", {"quadrilateral", 1}}};

    // Convert XDMF cell type string to DOLFIN cell type string
    std::string cell_type = type_attr.as_string();
    // boost::algorithm::to_lower(cell_type);
    auto it = xdmf_to_dolfin.find(cell_type);
    if (it == xdmf_to_dolfin.end())
    {
        throw std::runtime_error("Cannot recognise cell type. Unknown value: "
                                 + cell_type);
    }
    return it->second;
}

XDMFFile::XDMFFile(std::string filename) {
    _filename=std::move(filename);
}

void XDMFFile::read_mesh() {

    // Load XML doc from file
    pugi::xml_document xml_doc;
    pugi::xml_parse_result result = xml_doc.load_file(_filename.c_str());
    assert(result);

    // Get XDMF node
    pugi::xml_node xdmf_node = xml_doc.child("Xdmf");
    assert(xdmf_node);

    // Get domain node
    pugi::xml_node domain_node = xdmf_node.child("Domain");
    assert(domain_node);

    // Get grid node
    pugi::xml_node grid_node = domain_node.child("Grid");
    assert(grid_node);

    // Get topology node
    pugi::xml_node topology_node = grid_node.child("Topology");
    assert(topology_node);

    // Get information node
    pugi::xml_node information_node = domain_node.child("Information");
    assert(information_node);

    // Get cell type
    const auto cell_type_str = get_cell_type(topology_node);
    const int degree = cell_type_str.second;
    std::cout << degree;

    // Get CDATA
    std::cout<< information_node.text().get()<<std::endl;
    pugi::xml_document doc;
    const char* source =  information_node.text().get();
    pugi::xml_parse_result result_tag = doc.load_string(source);
    assert(result_tag);

    pugi::xml_node main_node = doc.child("main");

    // Creation of Map
    std::map<std::string, int> mapOfTag;
    for (pugi::xml_node child: main_node.children())
    {
        auto tag_key = child.first_attribute().value();
        int tag_value = atoi (child.child_value())  ;
        std::cout << "Key: " << tag_key<<std::endl   ;
        std::cout << "Value: " <<tag_value<<std::endl   ;

        // Insert Element in map
        mapOfTag.insert(std::pair<std::string, int>(tag_key, tag_value));
    }

    // Create a map iterator and point to beginning of map
    auto it = mapOfTag.begin();

    // Iterate over the map using Iterator till end.
    while (it != mapOfTag.end()) {
        // Accessing KEY from element pointed by it.
        std::string word = it->first;

        // Accessing VALUE from element pointed by it.
        int number = it->second;

        std::cout << word << " :: " << number << std::endl;

        // Increment the Iterator to point to next entry
        it++;
    }
}

XDMFFile::~XDMFFile() {

}

template <typename T>mesh::MeshValueCollection<T>
XDMFFile::read_mesh_value_collection(std::shared_ptr<const mesh::Mesh> mesh,
                                     std::string name) const
{
  // Load XML doc from file
  pugi::xml_document xml_doc;
  pugi::xml_parse_result result = xml_doc.load_file(_filename.c_str());
  assert(result);

  // Get XDMF node
  pugi::xml_node xdmf_node = xml_doc.child("Xdmf");
  assert(xdmf_node);

  // Get domain node
  pugi::xml_node domain_node = xdmf_node.child("Domain");
  assert(domain_node);

  // Check all Grid nodes for suitable dataset
  pugi::xml_node grid_node;
  for (pugi::xml_node node : domain_node.children("Grid"))
  {
    pugi::xml_node value_node = node.child("Attribute");
    if (value_node
        and (name == "" or name == value_node.attribute("Name").as_string()))
    {
      grid_node = node;
      break;
    }
  }

  // Get MVC topology node
  pugi::xml_node topology_node = grid_node.child("Topology");
  assert(topology_node);

  // Get description of MVC cell type and dimension from topology node
  auto cell_type_str = xdmf_utils::get_cell_type(topology_node);
  assert(cell_type_str.second == 1);
  std::unique_ptr<mesh::CellType> cell_type(
      mesh::CellType::create(cell_type_str.first));
  assert(cell_type);
  const int dim = cell_type->dim();
  const int num_verts_per_entity = cell_type->num_vertices();

  // Read MVC topology
  pugi::xml_node topology_data_node = topology_node.child("DataItem");
  assert(topology_data_node);
  boost::filesystem::path xdmf_filename(_filename);
  const boost::filesystem::path parent_path = xdmf_filename.parent_path();
  std::vector<std::int32_t> topology_data
      = xdmf_read::get_dataset<std::int32_t>(_mpi_comm.comm(),
                                             topology_data_node, parent_path);

  // Read values associated with each mesh::MeshEntity described by topology
  pugi::xml_node attribute_node = grid_node.child("Attribute");
  assert(attribute_node);
  pugi::xml_node attribute_data_node = attribute_node.child("DataItem");
  assert(attribute_data_node);
  std::vector<T> values_data = xdmf_read::get_dataset<T>(
      _mpi_comm.comm(), attribute_data_node, parent_path);

  // Ensure the mesh dimension is initialised
  mesh->create_entities(dim);
  const std::size_t global_vertex_range = mesh->num_entities_global(0);
  const std::int32_t num_processes = _mpi_comm.size();

  // Send entities to processes based on the lowest vertex index
  std::vector<std::vector<std::int32_t>> send_entities(num_processes);
  std::vector<std::vector<std::int32_t>> recv_entities(num_processes);

  std::vector<std::int32_t> v(num_verts_per_entity);
  for (auto& m : mesh::MeshRange<mesh::MeshEntity>(*mesh, dim))
  {
    if (dim == 0)
      v[0] = m.global_index();
    else
    {
      v.clear();
      for (auto& vtx : mesh::EntityRange<mesh::Vertex>(m))
        v.push_back(vtx.global_index());
      std::sort(v.begin(), v.end());
    }

    std::size_t dest
        = MPI::index_owner(_mpi_comm.comm(), v[0], global_vertex_range);
    send_entities[dest].push_back(m.index());
    send_entities[dest].insert(send_entities[dest].end(), v.begin(), v.end());
  }
  MPI::all_to_all(_mpi_comm.comm(), send_entities, recv_entities);

  // Map from {entity vertex indices} to {process, local_index}
  std::map<std::vector<std::int32_t>, std::vector<std::int32_t>> entity_map;
  for (std::int32_t i = 0; i != num_processes; ++i)
  {
    for (auto it = recv_entities[i].begin(); it != recv_entities[i].end();
         it += (num_verts_per_entity + 1))
    {
      std::copy(it + 1, it + num_verts_per_entity + 1, v.begin());
      auto map_it = entity_map.insert({v, {i, *it}});
      if (!map_it.second)
      {
        // Entry already exists, add to it
        map_it.first->second.push_back(i);
        map_it.first->second.push_back(*it);
      }
    }
  }

  // Send data from mesh::MeshValueCollection to sorting process
  std::vector<std::vector<T>> send_data(num_processes);
  std::vector<std::vector<T>> recv_data(num_processes);
  // Reset send/recv arrays
  send_entities = std::vector<std::vector<std::int32_t>>(num_processes);
  recv_entities = std::vector<std::vector<std::int32_t>>(num_processes);

  std::int32_t i = 0;
  for (auto it = topology_data.begin(); it != topology_data.end();
       it += num_verts_per_entity)
  {
    std::partial_sort_copy(it, it + num_verts_per_entity, v.begin(), v.end());
    std::size_t dest
        = MPI::index_owner(_mpi_comm.comm(), v[0], global_vertex_range);
    send_entities[dest].insert(send_entities[dest].end(), v.begin(), v.end());
    send_data[dest].push_back(values_data[i]);
    ++i;
  }

  MPI::all_to_all(_mpi_comm.comm(), send_entities, recv_entities);
  MPI::all_to_all(_mpi_comm.comm(), send_data, recv_data);

  // Reset send arrays
  send_data = std::vector<std::vector<T>>(num_processes);
  send_entities = std::vector<std::vector<std::int32_t>>(num_processes);

  // Locate entity in map, and send back to data to owning processes
  for (std::int32_t i = 0; i != num_processes; ++i)
  {
    assert(recv_data[i].size() * num_verts_per_entity
           == recv_entities[i].size());

    for (std::size_t j = 0; j != recv_data[i].size(); ++j)
    {
      auto it = recv_entities[i].begin() + j * num_verts_per_entity;
      std::copy(it, it + num_verts_per_entity, v.begin());
      auto map_it = entity_map.find(v);

      if (map_it == entity_map.end())
      {
        throw std::runtime_error("Cannotfind entity in map. "
                                 "Error reading mesh::MeshValueCollection");
      }
      for (auto p = map_it->second.begin(); p != map_it->second.end(); p += 2)
      {
        const std::int32_t dest = *p;
        assert(dest < num_processes);
        send_entities[dest].push_back(*(p + 1));
        send_data[dest].push_back(recv_data[i][j]);
      }
    }
  }

  // Send to owning processes and set in mesh::MeshValueCollection
  MPI::all_to_all(_mpi_comm.comm(), send_entities, recv_entities);
  MPI::all_to_all(_mpi_comm.comm(), send_data, recv_data);

  mesh::MeshValueCollection<T> mvc(mesh, dim);
  for (std::int32_t i = 0; i != num_processes; ++i)
  {
    assert(recv_entities[i].size() == recv_data[i].size());
    for (std::size_t j = 0; j != recv_data[i].size(); ++j)
    {
      mvc.set_value(recv_entities[i][j], recv_data[i][j]);
    }
  }

  return mvc;
}


// define a module to be imported by python
PYBIND11_MODULE(pybindings, mymodule) {
// export the XDMFFile class
pybind11::class_<XDMFFile>(mymodule, "XDMFFile")
.def(pybind11::init<std::string>())
.def("read_mesh", &XDMFFile::read_mesh);
}