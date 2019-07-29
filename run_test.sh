#! /bin/bash

while getopts abcd option
do
case "${option}"
in
a) 
cd  Scripts/cpp/mesh-workflow-no-xdmf/build/ && \
#cd  Scripts/cpp/mesh-workflow-new/build/ && \
	#cmake ..
	make -j 2

./demo_mesh_workflow;;

#cd ../../../python
#python3 poisson_subdomain.py;;
b) 
cd  Scripts/cpp/io-xdmf/build/ && \
	cmake ..
	make -j 2
	mpirun -n 2 ./io_xdmf;;
c)cd  Scripts/cpp/mesh-workflow-new/build/ && \
	#cmake ..
	make -j 2

./demo_mesh_workflow ;;
d) ;;
esac
done
