#! /bin/bash

cd  Scripts/cpp/mesh-workflow-no-xdmf/build/ && \
#cd  Scripts/cpp/mesh-workflow-new/build/ && \
	#cmake ..
	make -j 2

./demo_mesh_workflow