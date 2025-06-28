#include "Model.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "Vector3.h"
#include "Vector4.h"
#include "tgaimage.h"
#include "Triangle.h"
#include "Line.h"
#include "basic_math.h"
#include <algorithm>
#include "Matrix3.h"
#include "Matrix4.h"

constexpr TGAColor white = { 255, 255, 255, 255 }; // attention, BGRA order
constexpr TGAColor green = { 0, 255,   0, 255 };
constexpr TGAColor red = { 0,   0, 255, 255 };
constexpr TGAColor blue = { 255, 128,  64, 255 };
constexpr TGAColor yellow = { 0, 200, 255, 255 };

Model::Model(const char* file) {
	std::ifstream file_stream(file);
	if (!file_stream.is_open()) {
		std::cout << "Could not open file." << std::endl;
		return;
	}

	std::string line;
	int id = -1;
	while (!file_stream.eof()) {
		std::getline(file_stream, line);
		std::istringstream part(line);
		std::string l;
		//part << line;
		//std::cout << part.str() << std::endl;

		if (line.compare(0, 2, "v ") == 0) {
			std::string x, y, z;
			part >> l;
			part >> x >> y >> z;
			Vec3 v(std::stof(x), std::stof(y), std::stof(z));
			vertices.push_back(v);
			//std::cout << x << " " << y << " " << z << std::endl;
			++id;
		}else if (line.compare(0, 2, "f ") == 0) {
			while (!part.eof()) {
				part >> l;
				if (l.find('/') < std::string::npos) {
					//std::string num(l.begin() + 2, l.begin() + l.find('/'));
					l = l.substr(0, l.find('/'));
					faces.push_back(std::stoi(l) - 1);
					//std::cout << part.str() << " ";
				}
			}
			//std::cout << std::endl;
		} 
	}
	std::cout << "# v: " << faces.size() / 3<< " f: " << faces.size() << std::endl;
	std::cout << "Vertices: " << vertices.size() << std::endl;
}

void Model::wire_frame(TGAImage& image, TGAColor color) {
	for (int i = 0; i < faces.size(); i += 3) {
		std::vector<Vec3> ABC;
		for (int j = 0; j < 3; ++j) ABC.push_back(vertices[faces[i + j]]);
		Triangle Tri(ABC, false);
		Tri.screen_coords(image.width(), image.height());
		Line::draw_line(Tri.v, Tri.u, image, color);
		Line::draw_line(Tri.u, Tri.w, image, color);
		Line::draw_line(Tri.w, Tri.v, image, color);
	}
}

void Model::rasterize_scan(TGAImage& image, TGAColor color) {
	for (int i = 0; i < faces.size(); i += 3) {
		std::vector<Vec3> ABC;
		//The model face values are the vertices. f v/vn/vt ... the three v's are a single face index at the vertices vector.
		for (int j = 0; j < 3; ++j) ABC.push_back(vertices[faces[i + j]]);
		Triangle Tri(ABC, false);
		Tri.sort();
		Tri.screen_coords(image.width(), image.height());
		std::vector<Vec3> left;
		std::vector<Vec3> right;
		accumulate_triangle(left, right, Tri);
		
		if (left.size() != right.size()) continue;
		TGAColor c;
		for (int k = 0; k < 3; ++k) c[k] = rand() % 255;
		for (int y = 0; y < left.size() - 1; ++y) {
			//if (left[y].x == right[y].x) std::cout << "weird" << std::endl;
			//if (std::abs(right[y].x - left[y].x) > 20) continue;
			Line::draw_line(left[y], right[y], image, c);
			//std::cout << right[y] << " " << left[y] << std::endl;
		}
	}
}

void Model::accumulate_triangle(std::vector<Vec3>& left, std::vector<Vec3>& right, Triangle& ABC) {
	left = interpolate(ABC.v, ABC.u);
	if(!left.empty()) left.pop_back();
	std::vector<Vec3> mid = interpolate(ABC.u, ABC.w);
	left.insert(left.end(), mid.begin(), mid.end());
	right = interpolate(ABC.v, ABC.w);
	//if (left.size() != right.size()) std::cout << left.size() << " " << right.size() << std::endl;
}

void Model::rasterize_bary(TGAImage& image, TGAColor color) {
	for (int i = 0; i < faces.size(); i += 3) {
		std::vector<Vec3> ABC;
		//The model face values are the vertices. f v/vn/vt ... the three v's are a single face index at the vertices vector.
		for (int j = 0; j < 3; ++j) ABC.push_back(vertices[faces[i + j]]);
		Triangle Tri(ABC, false);
		TGAColor c;
		for (int k = 0; k < 3; ++k) c[k] = rand() % 255;
		Tri.screen_coords(image.width(), image.height());
		float total_area = signed_area(Tri.v, Tri.u, Tri.w);
		//std::cout << total_area << std::endl;
		bound_fill(Tri, total_area, image, c);
	}
}

void Model::rasterize_bary(TGAImage& image, TGAColor color, std::vector<float>& z_buffer) {
	for (int i = 0; i < faces.size(); i += 3) {
		std::vector<Vec3> ABC;
		//The model face values are the vertices. f v/vn/vt ... the three v's are a single face index at the vertices vector.
		for (int j = 0; j < 3; ++j) ABC.push_back(vertices[faces[i + j]]);
		Triangle Tri(ABC, false);
		TGAColor c;
		for (int k = 0; k < 3; ++k) c[k] = rand() % 255;
		Tri.screen_coords(image.width(), image.height());
		float total_area = signed_area(Tri.v, Tri.u, Tri.w);
		//std::cout << total_area << std::endl;
		bound_fill(Tri, total_area, image, c, z_buffer);
	}
}

void Model::bound_fill(Triangle& T, float total_area, TGAImage& image, TGAColor c) {
	if (total_area <= 0) return;
	//Vec3 light(0, 1, 0);
	for (int x = T.low_x; x <= T.high_x; ++x) {
		for (int y = T.low_y; y <= T.high_y; ++y) {
			Vec3 P(x, y, 0);
			float alpha = signed_area(P, T.u, T.w) / total_area;
			float beta = signed_area(P, T.w, T.v) / total_area;
			float gamma = signed_area(P, T.v, T.u) / total_area;
			if (alpha < 0 || beta < 0 || gamma < 0) continue;
			float z = alpha * T.v.z + beta * T.u.z + gamma * T.w.z;
			//std::cout << z << std::endl;
			//P = P.normalize();
			//float intensity = std::clamp(light.normalize().dot(P), 0.f, 1.f);
			//std::cout << intensity << std::endl;
			TGAColor l{ 255 * z, 255 * z, 255 * z, 255 };
			if (z > 0 ) {
				image.set(x, y, l);
			}
		}
	}
}

void Model::bound_fill(Triangle& T, float total_area, TGAImage& image, TGAColor c, std::vector<float>& z_buffer) {
	if (total_area <= 0) return;
	//std::cout << z_buffer.size() << std::endl;
	//Vec3 light(0, 1, 0);
	for (int x = T.low_x; x <= T.high_x; ++x) {
		for (int y = T.low_y; y <= T.high_y; ++y) {
			Vec3 P(x, y, 0);
			float alpha = signed_area(P, T.u, T.w) / total_area;
			float beta = signed_area(P, T.w, T.v) / total_area;
			float gamma = signed_area(P, T.v, T.u) / total_area;
			if (alpha < 0 || beta < 0 || gamma < 0) continue;
			float z = alpha * T.v.z + beta * T.u.z + gamma * T.w.z;
			z = (z + 1.0f) * 0.5f;
			//std::cout << z << std::endl;
			//P = P.normalize();
			//float intensity = std::clamp(light.normalize().dot(P), 0.f, 1.f);
			//std::cout << intensity << std::endl;
			//if (z < 0 || z > 1) std::cout << "Z out of bounds: " << z << " at pixel (" << x << "," << y << ")\n";
			//uint8_t shade = static_cast<uint8_t>(std::clamp(z * 255.f, 0.f, 255.f));
			TGAColor l{ z * 255.f, z * 255.f, z * 255.f, 255 };
			int index = x + y * image.width();
			//std::cout << x + y * image.width() << std::endl;
			if (index < z_buffer.size() && z > z_buffer[index]) {
				//std::cout << z << std::endl;
				Mat4 p;
				p = p.perspective(image.width(), image.height(), -1.f, 1.f, 90.f);
				Vec3 v(x, y, z);
				//v = p * v;
				image.set(v.x, v.y, l);
				z_buffer[index] = z;
			}
			
		}
	}
}

void Model::rotate_matrix(TGAImage& frame, TGAColor color, std::vector<float>& z_buffer) {
	Vec3 v(0, 45, 0);
	Mat3 r = Mat3::rotate(v);
	Mat4 p;
	p = p.perspective(frame.width(), frame.height(), .1f, 1.f, 45.f);
	Vec3 position(30, 40, 100);
	std::cout << p << std::endl;
	for (int i = 0; i < faces.size(); i += 3) {
		std::vector<Vec3> ABC;
		//The model face values are the vertices. f v/vn/vt ... the three v's are a single face index at the vertices vector.
		for (int j = 0; j < 3; ++j) ABC.push_back(r * vertices[faces[i + j]]);

		Triangle Tri(ABC, false);
		TGAColor c;

		for (int k = 0; k < 3; ++k) c[k] = rand() % 255;

		Tri.screen_coords(frame.width(), frame.height());
		Vec3 vt = Mat4::translate(position, Tri.v);
		Vec3 ut = Mat4::translate(position, Tri.u);
		Vec3 wt = Mat4::translate(position, Tri.w);
		
		Vec4 v = p * Tri.v;
		Vec4 u = p * Tri.u;
		Vec4 w = p * Tri.w;

		//std::cout << v << " " << u << " " << w << std::endl;
		std::cout << vt << " " << ut << " " << wt << std::endl;

		v = v / v.w;
		u = u / u.w;
		w = w / w.w;

		/*std::cout << v / v.w << " " << u / u.w << " " << w / w.w << std::endl;
		std::cout << Tri.v << " " << Tri.u << " " << Tri.w << std::endl;*/
		Tri.v = v;
		Tri.u = u;
		Tri.w = w;

		//std::cout << Tri.v << " " << Tri.u << " " << Tri.w << std::endl;

		float total_area = signed_area(Tri.v, Tri.u, Tri.w);

		//std::cout << total_area << std::endl;
		bound_fill(Tri, total_area, frame, c, z_buffer);
	}
}