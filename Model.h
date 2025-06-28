#pragma once
#include <vector>
class Vec3;
class TGAImage;
struct TGAColor;
class Triangle;


class Model {
private:

public:
	Model(const char* filename);
	void wire_frame(TGAImage& image, TGAColor color);
	void rasterize_scan(TGAImage& image, TGAColor color);
	void rasterize_bary(TGAImage& image, TGAColor color);
	void rasterize_bary(TGAImage& image, TGAColor color, std::vector<float>& z_buffer);
	void bound_fill(Triangle& T, float total_area, TGAImage& image, TGAColor c);
	void bound_fill(Triangle& T, float total_area, TGAImage& image, TGAColor c, std::vector<float>& z_buffer);
	std::vector<Vec3> vertices;
	std::vector<int> faces;
	void rotate_matrix(TGAImage& frame, TGAColor c, std::vector<float>& z_buffer);

private:
	void accumulate_triangle(std::vector<Vec3>& left, std::vector<Vec3>& right, Triangle& ABC);
	
};
