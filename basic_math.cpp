#include "Math.h"
#include "Vector3.h"


std::vector<Vec3> interpolate(Vec3 v, Vec3 u) {
	//The slope must be inversed because we want to interpolate with respect with y.
	float slope = 0;

	float dy = (u.y - v.y);
	float dx = (u.x - v.x);
	
	//x is our starting point for these two points. It is assumed that v.y < u.y for this operation.
	float x = v.x;
	int increment = v.x < u.x ? 1 : -1;
	std::vector<Vec3> vec;
	for (int y = v.y; y <= u.y; ++y) {
		vec.push_back(Vec3(x, (float)y, 0.f));
		/*if (slope > dy) {
			slope -= dy;
			x += increment;
		}
		slope += dx;*/
		x += dx / dy;
	}

	return vec;
}

Vec3 barycentric(Vec3& A, Vec3& B, Vec3& C, Vec3& P) {

	Vec3 AB = B - A;
	Vec3 AC = C - A;
	Vec3 AP = P - A;
	Vec3 BP = P - B;
	Vec3 CP = P - C;
	float areaABC = .5f * Vec3::magnitude(Vec3::cross_product(AB, AC));

	if (std::abs(areaABC) < 1e-6f) return Vec3(-1, -1, -1); // Explicit early exit
	areaABC = 1 / areaABC;

	float alpha = .5f *  Vec3::magnitude(Vec3::cross_product(BP, CP)) * areaABC;
	float beta = .5f * Vec3::magnitude(Vec3::cross_product(CP, AP)) * areaABC;
	float gamma = 1 - alpha - beta;
	
	return Vec3(gamma, beta, alpha); // Interpolated point		
}

float signed_area(Vec3 v, Vec3 u, Vec3 w) {
	return .5f * ((u.y * v.x - u.x * v.y) + (u.x * w.y - u.y * w.x) + (w.x * v.y - v.x * w.y));
}
