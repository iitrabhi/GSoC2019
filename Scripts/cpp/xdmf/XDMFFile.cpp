#include <utility>

#include "XDMFFile.h"
#include "pugixml.hpp"
#include <iostream>
#include <assert.h>
#include <map>

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

