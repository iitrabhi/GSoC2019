#include "XDMFFile.h"
int main(){
    XDMFFile xdmfFile("tag_all.xdmf");
    xdmfFile.read_mesh();
    return 0;
}