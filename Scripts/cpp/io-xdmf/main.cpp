#include <cfloat>
#include <dolfin.h>
#include "main.h"
using namespace dolfin;
int main(int argc, char* argv[])
{
	common::SubSystemsManager::init_logging(argc, argv);
  	common::SubSystemsManager::init_petsc(argc, argv);

  	//------------------Constructor---------------------------
	dolfin::io::XDMFFile xdmf_file(MPI_COMM_WORLD,"../input/unit_square.xdmf");
	//------------------read_mesh---------------------------
	auto mesh = std::make_shared<mesh::Mesh>(xdmf_file.read_mesh(MPI_COMM_WORLD,mesh::GhostMode::none));
	//------------------read_information---------------------------
	auto map_of_info = xdmf_file.read_information();
	//------------------print_information---------------------------
	std::cout<<"Tags: "<<std::endl;
	auto it = map_of_info.begin();

	while (it != map_of_info.end()) {
	  std::string word = it->first;
	  int number = it->second;
	  std::cout << word << " :: " << number << std::endl;
	  it++;
	}
	//------------------write mesh---------------------------
	dolfin::io::XDMFFile xdmf_file_2(MPI_COMM_WORLD,"../input/unit_square_from_dolfin.xdmf");
	xdmf_file_2.write(*mesh);
	xdmf_file_2.write(map_of_info);
	return 0;
	//------------------write function---------------------------
	//io::XDMFFile file(MPI_COMM_WORLD,"u.xdmf");
  	//file.write(u);
}

