#pragma once
#include <iostream>

class Vec4;
class Vec3;
class Mat3;

class Mat4 {
public:
	Mat4() : m{} {};
	Mat4(float x);
	Mat4(Vec4 v, Vec4 u, Vec4 w, Vec4 r);
	Mat4(Mat3& matrix);

	float* operator[](const int i) { return m[i]; }
	const float* operator[](const int i) const { return m[i]; }

	Mat4 operator*(const Mat4& A);
	Mat4 operator*(const Mat4&& A);
	Vec4 operator*(const Vec3& v);
	Vec4 operator*(const Vec4& v);

	Mat4 operator/(const float f);
	Mat4 operator*(const float f);

	friend std::ostream& operator<<(std::ostream& os, Mat4& mat) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j){
				os << mat.m[i][j] << " ";
			}
			os << std::endl;
		}
		return os;
	}

	friend std::ostream& operator<<(std::ostream& os, Mat4&& mat) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				os << mat.m[i][j] << " ";
			}
			os << std::endl;
		}
		return os;
	}

	float determinant() const;
	Mat4 cofactor() const;
	Mat4 transpose();
	Mat4 inverse() const;
	Mat4 perspective(float width, float height, float near, float far, float fov);
	static Vec3 translate(Vec3 position, Vec3 original);

	float m[4][4]{};
};

