//+
Point(1) = {0, 0, 0, 1.0};
//+
Point(2) = {0, 1, 0, 1.0};
//+
Point(3) = {1, 1, 0, 1.0};
//+
Point(4) = {1, 0, 0, 1.0};
//+
Line(1) = {1, 4};
//+
Line(2) = {4, 3};
//+
Line(3) = {3, 2};
//+
Line(4) = {2, 1};
//+
Curve Loop(1) = {3, 4, 1, 2};
//+
Plane Surface(1) = {1};
//+
Physical Curve("BOTTOM") = {1};
//+
Physical Curve("TOP") = {3};
//+
Physical Curve("LEFT") = {4};
//+
Physical Curve("RIGHT") = {2};
//+
Physical Surface("DOMAIN") = {1};
