#include <cfloat>
#include <dolfin.h>

using namespace dolfin;

int main(int argc, char* argv[])
{
  common::SubSystemsManager::init_logging(argc, argv);
  common::SubSystemsManager::init_petsc(argc, argv);

  dolfin::io::XDMFFile xdmf_file(MPI_COMM_WORLD,"input/tag_all.xdmf");
  auto mesh_2d = std::make_shared<mesh::Mesh>(xdmf_file.read_mesh(MPI_COMM_WORLD,mesh::GhostMode::none));

  auto mapOfTag = xdmf_file.read_tags();

  std::cout<<"Tags: "<<std::endl;

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
  //std::cout<<"Hmin: "<< mesh_2d->hmin() <<std::endl;
//
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
