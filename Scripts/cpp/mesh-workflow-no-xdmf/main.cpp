#include <cfloat>
#include <dolfin.h>
#include "main.h"
using namespace dolfin;
int main(int argc, char* argv[])
{ 

  auto dummy_mesh = DummyMesh();
  auto vect =dummy_mesh.points;                          

  common::SubSystemsManager::init_logging(argc, argv);
  common::SubSystemsManager::init_petsc(argc, argv);

  dolfin::io::XDMFFile xdmf_file(MPI_COMM_WORLD,"../input/mesh_2d.xdmf");
  auto mesh_2d = std::make_shared<mesh::Mesh>(xdmf_file.read_mesh(MPI_COMM_WORLD,mesh::GhostMode::none));

  
  std::cout<<mesh_2d->topology().str(true);
 
  auto mvc = dolfin::mesh::MeshValueCollection<int>(mesh_2d,dummy_mesh.line_no,dummy_mesh.cell_data,1);
  std::cout<<"MVC size: "<< mvc.size() <<std::endl;
  auto mvcval = mvc.values();

  std::cout<< "MVC Values" <<std::endl;
  // Create a map iterator and point to beginning of map
  auto it = mvcval.begin();

  // Iterate over the map using Iterator till end.
  while (it != mvcval.end()) {
      // Accessing KEY from element pointed by it.
      auto mapcell = it->first;

      // Accessing VALUE from element pointed by it.
      int number = it->second;

      std::cout << "(" << mapcell.first << ", " << mapcell.second<< ")" <<" :: " << number << std::endl;

      // Increment the Iterator to point to next entry
      it++;
  }

  std::cout<<std::endl;

  //dolfin::io::XDMFFile xdmf_file_mvc(MPI_COMM_WORLD,"output/mvc_1d.xdmf");
  //auto mvc = xdmf_file_mvc.read_mvc_int(mesh_2d,"name_to_read");

  auto mf = dolfin::mesh::MeshFunction<int>(mesh_2d,mvc,1);
  auto val = mf.values();

  std::cout<<"MF Dimension: "<< mf.dim() <<std::endl;
  std::cout<<"MF Size: "<< mf.size() <<std::endl;
  
  std::cout<< "MF Values" <<std::endl;
  for(int i=0;i<mf.size();i++){
      if(i%20==0){
          std::cout<<std::endl;
      }else{
          std::cout<< val[i] <<"    ";
      }
  }
  std::cout<<std::endl;
  return 0;
}

