#include <cfloat>
#include <dolfin.h>
#include "main.h"

using namespace dolfin;

int main(int argc, char *argv[]) {
    auto show_connectivity = false;
    auto show_gmsh_dolfin_map = false;

    auto dummy_mesh = DummyMesh();
    auto vect = dummy_mesh.points;


    std::map<std::vector<double>, size_t> co_ord_point_map;

    common::SubSystemsManager::init_logging(argc, argv);
    common::SubSystemsManager::init_petsc(argc, argv);

    dolfin::io::XDMFFile xdmf_file(MPI_COMM_WORLD, "../input/mesh_2d.xdmf");
    auto mesh_2d = std::make_shared<mesh::Mesh>(xdmf_file.read_mesh(mesh::GhostMode::none));


    // std::cout<<mesh_2d->topology().str(true);

    if (show_connectivity) {
        std::vector<std::vector<double>> points = vect;
        // A map from co_ordinates to point based on input array
        //std::cout<<"Points size: "<<points.size()<< std::endl;
        for (int i = 0; i < points.size(); ++i) {
            std::vector<double> ve = points[i];
            co_ord_point_map.insert(std::make_pair(ve, i));
        }
    }

    if (show_connectivity) {
        mesh_2d->create_connectivity(1, 0);// connectivity in a 2d geo of lines
        auto connectivity = mesh_2d->topology().connectivity(1, 0);
        //std::cout<<"Connectivity: "<< connectivity->size(1)<<std::endl;

        std::map<std::vector<int>, size_t> vertex_edge_map;

        for (int i = 0; i < mesh_2d->topology().size(1); ++i) {
            auto connection = connectivity->connections(i);
            std::vector<int> ve{connection[0], connection[1]};
            vertex_edge_map.insert(std::make_pair(ve, i));

            //std::cout<<"Connectivity: ["<<i<<"]"<< connection[0]<<"::"<<connection[1]<<std::endl;
        }
        auto itr = vertex_edge_map.find({17, 5});
        if (itr != std::end(vertex_edge_map)) {
            int v = itr->second;
            std::cout << "Edge find: " << v << std::endl;
        }
    }

    if (show_gmsh_dolfin_map) {
        int dim = 1;
        std::vector<std::int32_t> v(2);
        std::map<std::vector<int>, size_t> vertex_edge_map;
        for (auto &m : mesh::MeshRange<mesh::MeshEntity>(*mesh_2d, 1)) {
            if (dim == 0)
                v[0] = m.global_index();
            else {
                v.clear();
                for (auto &vtx : mesh::EntityRange<mesh::Vertex>(m))
                    v.push_back(vtx.global_index());
                std::sort(v.begin(), v.end());
            }

            vertex_edge_map[v] = m.index();
            std::cout << "Edge: " << m.index() << "::" << v[0] << ":" << v[1] << std::endl;
        }
        auto map_it = vertex_edge_map.find({3, 13});
        std::cout << "Edge Number:" << map_it->second << std::endl;
    }


    if (true) {

        auto mvc = dolfin::mesh::MeshValueCollection<int>(mesh_2d, 1,
                                                          dummy_mesh.cells, dummy_mesh.cell_data);

        std::cout << "MVC size: " << mvc.size() << std::endl;
        auto mvcval = mvc.values();

        std::cout << "MVC Values" << std::endl;
        // Create a map iterator and point to beginning of map
        auto it = mvcval.begin();

        // Iterate over the map using Iterator till end.
        while (it != mvcval.end()) {
            // Accessing KEY from element pointed by it.
            auto mapcell = it->first;

            // Accessing VALUE from element pointed by it.
            int number = it->second;

            std::cout << "(" << mapcell.first << ", " << mapcell.second << ")" << " :: " << number << std::endl;

            // Increment the Iterator to point to next entry
            it++;
        }

        std::cout << std::endl;

        //dolfin::io::XDMFFile xdmf_file_mvc(MPI_COMM_WORLD,"output/mvc_1d.xdmf");
        //auto mvc = xdmf_file_mvc.read_mvc_int(mesh_2d,"name_to_read");

        auto mf = dolfin::mesh::MeshFunction<int>(mesh_2d, mvc, 1);
        auto val = mf.values();

        std::cout << "MF Dimension: " << mf.dim() << std::endl;
        std::cout << "MF Size: " << mf.size() << std::endl;

        std::cout << "MF Values" << std::endl;
        for (int i = 0; i < mf.size(); i++) {
            if (i % 20 == 0) {
                std::cout << std::endl;
            } else {
                std::cout << val[i] << "    ";
            }
        }
        std::cout << std::endl;
    }
    return 0;
}

