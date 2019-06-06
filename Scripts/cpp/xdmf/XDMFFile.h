#include <string>
#include "pugixml.hpp"

// Get DOLFIN cell type string from XML topology node
std::pair<std::string, int> get_cell_type(const pugi::xml_node& topology_node);


class XDMFFile {
    std::string _filename;

public:
    /// Constructor
    explicit XDMFFile( std::string filename);

    /// Destructor
    ~XDMFFile();

    void read_mesh() ;
};