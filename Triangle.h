#pragma once
#include "Vector3.h"
#include <iostream>
#include <vector>

class Triangle {
public:
	Triangle(Vec3 v, Vec3 u, Vec3 w, bool is_screen) : v(v), u(u), w(w), is_screen(is_screen) {
		bound();
	}

	Triangle(std::vector<Vec3> ABC, bool is_screen) : v(ABC[0]), u(ABC[1]), w(ABC[2]), is_screen(is_screen) {
		bound();
	}

	void bound() {
		low_x = std::min({ v.x, u.x, w.x });
		high_x = std::max({ v.x, u.x, w.x });

		low_y = std::min({ v.y, u.y, w.y });
		high_y = std::max({ v.y, u.y, w.y });
	}

	//Only sort for scanline not barycentric
	void sort() {
		//Triangles sorted for rasterization fill later.
		if (v.y > u.y) std::swap(v, u);
		if (v.y > w.y) std::swap(v, w);
		if (u.y > w.y) std::swap(u, w);
	}

	void screen_coords(int width, int height) {
		if (is_screen) return;
		v.x = (v.x + 1) * (width / 2.f);
		v.y = (v.y + 1) * (height / 2.f);

		u.x = (u.x + 1) * (width / 2.f);
		u.y = (u.y + 1) * (height / 2.f);

		w.x = (w.x + 1) * (width / 2.f);
		w.y = (w.y + 1) * (height / 2.f);
		bound();
		//sort();

		is_screen = true;
	}

	void normalized_coords(int width, int height) {
		if (!is_screen) return;
		v.x = (2 * v.x / width) - 1;
		v.y = (2 * v.y / height) - 1;

		u.x = (2 * u.x / width) - 1;
		u.y = (2 * u.y / height) - 1;

		w.x = (2 * w.x / width) - 1;
		w.y = (2 * w.y / height) - 1;
		bound();
		//sort();

		is_screen = false;
	}

	Vec3 v;
	Vec3 u;
	Vec3 w;

	float low_y; 
	float high_y;

	float low_x;
	float high_x;

	bool is_screen;
};