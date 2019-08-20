// Gmsh project created on Mon Aug 19 15:54:05 2019
SetFactory("OpenCASCADE");
// Inputs
squareSide = 1; //m
gridsize = squareSide / 10;

// All numbering counterclockwise from bottom-left corner
Point(1) = {-squareSide/2, -squareSide/2, 0, gridsize};
Point(2) = {squareSide/2, -squareSide/2, 0, gridsize};
Point(3) = {squareSide/2, squareSide/2, 0, gridsize};
Point(4) = {-squareSide/2, squareSide/2, 0, gridsize};
Line(1) = {1, 2};				// bottom line
Line(2) = {2, 3};				// right line
Line(3) = {3, 4};				// top line
Line(4) = {4, 1};				// left line
Line Loop(5) = {1, 2, 3, 4}; 	
// the order of lines in Line Loop is used again in surfaceVector[]
Plane Surface(6) = {5};
Transfinite Surface {6} = {2,3,4,1};
Transfinite Line {1, 3} = 4 Using Progression 1;
Transfinite Line {2, 4} = 4 Using Progression 1;
Recombine Surface {6};

surfaceVector[] = Extrude {0, 0, 1} {
	 Surface{6};
	 Layers{4};
	 Recombine;
	};

Physical Volume("box") = {1};
