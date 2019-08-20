// This code was created by pygmsh v6.0.2.
SetFactory("OpenCASCADE");
p0 = newp;
Point(p0) = {0, 0, 0, 0.5};
p1 = newp;
Point(p1) = {1, 0, 0, 0.5};
p2 = newp;
Point(p2) = {1, 1, 0, 0.5};
p3 = newp;
Point(p3) = {0, 1, 0, 0.5};
l0 = newl;
Line(l0) = {p0, p1};
l1 = newl;
Line(l1) = {p1, p2};
l2 = newl;
Line(l2) = {p2, p3};
l3 = newl;
Line(l3) = {p3, p0};
ll0 = newll;
Line Loop(ll0) = {l0, l1, l2, l3};
s0 = news;
Plane Surface(s0) = {ll0};
Transfinite Line {l0, l2} = 4;
Transfinite Line {l1, l3} = 4;
Transfinite Surface {s0};
ex1[] = Extrude {0,0,1} {Surface{s0}; Layers{4}; };
Physical Point("POINT_LEFT") = {p0, p3};
Physical Point("POINT_RIGHT") = {p1, p2};
Physical Line("LINE_X") = {l0, l2};
Physical Line("LINE_Y") = {l1, l3};
Physical Surface("SURFACE") = {s0};
Physical Volume("BOX") = {ex1[1]};