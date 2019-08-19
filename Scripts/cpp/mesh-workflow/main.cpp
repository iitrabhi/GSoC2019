#include <cfloat>
#include <dolfin.h>

using namespace dolfin;

int main(int argc, char* argv[])
{
  common::SubSystemsManager::init_logging(argc, argv);
  common::SubSystemsManager::init_petsc(argc, argv);

  // dolfin::io::XDMFFile xdmf_file(MPI_COMM_WORLD,"../input/mesh_2d.xdmf");
  // auto mesh_2d = std::make_shared<mesh::Mesh>(xdmf_file.read_mesh(mesh::GhostMode::none));
  
  // std::cout<< mesh_2d->hmin() <<std::endl;

  // Create mesh and function space
  std::array<Eigen::Vector3d, 2> pt{Eigen::Vector3d(0.0, 0.0, 0.0),
                                    Eigen::Vector3d(1.0, 1.0, 1.0)};
  auto _mesh = std::make_shared<mesh::Mesh>(generation::BoxMesh::create(
      MPI_COMM_WORLD, pt, {{32, 32, 32}}, mesh::CellType::hexahedron,
      mesh::GhostMode::none));
  
  auto entity_index = 0;
  auto _dim = 2;
  const std::size_t D = _mesh->topology().dim();

  _mesh->create_entities(_dim);

  _mesh->create_connectivity(_dim, D);
  assert(_mesh->topology().connectivity(_dim, D));
  const mesh::Connectivity& connectivity = *_mesh->topology().connectivity(_dim, D);


  const mesh::MeshEntity entity(*_mesh, _dim, entity_index);

  dolfin::mesh::CellType type = _mesh->cell_type;
  const std::int32_t num_vertices_per_cell = mesh::num_cell_vertices(type);

  std::cout<< num_vertices_per_cell <<std::endl;

  // dolfin::io::XDMFFile xdmf_file_mvc(MPI_COMM_WORLD,"../input/mvc_1d.xdmf");
  // auto mvc = xdmf_file_mvc.read_mvc_int(mesh_2d,"name_to_read");
// 
  // //std::cout<< mvc.size() <<std::endl;
// 
  // auto mf = dolfin::mesh::MeshFunction<int>(mesh_2d,mvc,1);
  // auto val = mf.values();
// 
  // std::cout<<"Dimension: "<< mf.dim() <<std::endl;
  // std::cout<<"Size: "<< mf.size() <<std::endl;
// 
  // std::cout<< "Values" <<std::endl;
  // for(int i=0;i<mf.size();i++){
  //     if(i%20==0){
  //         std::cout<<std::endl;
  //     }else{
  //         std::cout<< val[i] <<"    ";
  //     }
  // }
  // std::cout<<std::endl;
  return 0;
}
