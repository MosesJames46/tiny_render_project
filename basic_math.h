#pragma once
#include <iostream>
#include <vector>

class Vec3;

std::vector<Vec3> interpolate(Vec3 v, Vec3 u);
Vec3 barycentric(Vec3& v, Vec3& u, Vec3& w, Vec3& p);
float signed_area(Vec3 v, Vec3 u, Vec3 w);
