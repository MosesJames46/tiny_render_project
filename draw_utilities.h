#pragma once
class Vec3;
class TGAImage;
class TGAColor;
class Triangle;
#include <vector>

void triangle(Vec3 v, Vec3 u, Vec3 w, TGAImage& image, TGAColor color);
void triangle_bary(Triangle triangle, TGAImage& image, TGAColor color, std::vector<float>& buffer);