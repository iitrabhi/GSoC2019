#include <cfloat>
#include <dolfin.h>
#include "main.h"
using namespace dolfin;
int main(int argc, char* argv[])
{ 
  //common::SubSystemsManager::init_logging(argc, argv);
  //common::SubSystemsManager::init_petsc(argc, argv);
 //auto dummy_mesh = DummyMesh();
 //auto vect =dummy_mesh.points;
 //auto rows=vect.size();
 //auto col=vect[0].size();
 //// Displaying the 2D vector 
 //Eigen::ArrayXXd points(rows, col)  ;
 //Eigen::ArrayXXd cells(rows, col)  ;
 //for (int i = 0; i < vect.size(); i++) { 
 //    for (int j = 0; j < vect[i].size(); j++){
 //      std::cout << vect[i][j] << " "; 
 //      points(i,j)=vect[i][j];
 //      cells(i,j)=dummy_mesh.cells[i][j];
 //    } 
 //    std::cout << std::endl; 
 //} 

  
  //const Eigen::Ref<const EigenRowArrayXXi64> cells = ;

  //std::int32_t num_ghost_cells = 0;
  //const std::vector<std::int64_t>& global_cell_indices{};
  //dolfin::mesh::Mesh mesh_2d(MPI_COMM_WORLD,
  //                            mesh::CellType::Type::triangle,
  //                            points,
  //                            cells,
  //                            global_cell_indices,
  //                            dolfin::mesh::GhostMode::none,
  //                            num_ghost_cells);                             

  common::SubSystemsManager::init_logging(argc, argv);
  common::SubSystemsManager::init_petsc(argc, argv);

  dolfin::io::XDMFFile xdmf_file(MPI_COMM_WORLD,"../input/mesh_2d.xdmf");
  auto mesh_2d = std::make_shared<mesh::Mesh>(xdmf_file.read_mesh(MPI_COMM_WORLD,mesh::GhostMode::none));

  
  std::cout<<mesh_2d->topology().str(true);
 

  std::cout<<"Hmin: "<< mesh_2d->hmin() <<std::endl;

  auto mvc = dolfin::mesh::MeshValueCollection<int>(mesh_2d,1);
  std::cout<<"MVC size: "<< mvc.size() <<std::endl;

  //dolfin::io::XDMFFile xdmf_file_mvc(MPI_COMM_WORLD,"output/mvc_1d.xdmf");
  //auto mvc = xdmf_file_mvc.read_mvc_int(mesh_2d,"name_to_read");
//
  //std::cout<<"MVC size: "<< mvc.size() <<std::endl;
  //auto mf = dolfin::mesh::MeshFunction<int>(mesh_2d,mvc,1);
  //auto val = mf.values();
//
  //std::cout<<"Dimension: "<< mf.dim() <<std::endl;
  //std::cout<<"Size: "<< mf.size() <<std::endl;
//
  //std::cout<< "Values" <<std::endl;
  //for(int i=0;i<mf.size();i++){
  //    if(i%20==0){
  //        std::cout<<std::endl;
  //    }else{
  //        std::cout<< val[i] <<"    ";
  //    }
  //}
  std::cout<<std::endl;
  return 0;
}

