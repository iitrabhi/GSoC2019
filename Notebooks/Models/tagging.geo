// Gmsh project created on Sat May 18 21:31:14 2019
SetFactory("OpenCASCADE");
//+
Point(1) = {0, 0, 0, lc};
//+
Point(2) = {1, 0, 0, lc};
//+
Point(3) = {1, 1, 0, lc};
//+
Point(4) = {0, 1, 0, lc};
//+
Line(1) = {1, 2};
//+
Line(2) = {2, 3};
//+
Line(3) = {3, 4};
//+
Line(4) = {4, 1};
//+
Curve Loop(1) = {3, 4, 1, 2};
//+
Plane Surface(1) = {1};
//+
Physical Curve("bottom") = {1};
//+
Physical Curve("right") = {2};
//+
Physical Curve("top") = {3};
//+
Physical Curve("left") = {4};
//+
Physical Surface("domain") = {1};
