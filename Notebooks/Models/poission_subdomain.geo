// Gmsh project created on Fri Jun 28 19:19:24 2019
SetFactory("OpenCASCADE");
lc = 0.2;
Point(1) = {0, 0, 0, lc};
Point(2) = {0, 1, 0, lc};
Point(3) = {1, 1, 0, lc};
Point(4) = {1, 0, 0, lc};
Point(5) = {0.2, 0.5, 0, lc};
Point(6) = {0.2, 0.7, 0, lc};
Point(7) = {1.0, 0.5, 0, lc};
Point(8) = {1.0, 0.7, 0, lc};

Line(1) = {1, 4};
Line(2) = {3, 2};
Line(3) = {2, 1};
Line(4) = {7, 5};
Line(5) = {5, 6};
Line(6) = {6, 8};
Line(7) = {4, 7};
Line(8) = {7, 8};
Line(9) = {8, 3};

Curve Loop(1) = {2, 3, 1, 7, 4, 5, 6, 9};
Plane Surface(1) = {1};
Curve Loop(2) = {6, -8, 4, 5};
Plane Surface(2) = {2};

Physical Curve("LEFT") = {3};
Physical Curve("TOP") = {2};
Physical Curve("RIGHT") = {9, 8, 7};
Physical Curve("BOTTOM") = {1};

Physical Surface("DOMAIN") = {1};
Physical Surface("OBSTACLE") = {2};
