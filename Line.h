#pragma once
#include <fstream>

class TGAImage;
struct TGAColor;
class Vec3;

class Line {
public:
	static void draw_line(int x0, int y0, int x1, int x2, TGAImage& image, TGAColor color);
	static void draw_line(Vec3 v, Vec3 u, TGAImage& image, TGAColor color);
};