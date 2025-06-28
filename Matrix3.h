#pragma once
#include <iostream>

class Vec3;

class Mat3 {
public:
	Mat3() = default;
	Mat3(Vec3 x, Vec3 y, Vec3 z);
	Mat3(const float i) {
		m[0][0] = i;
		m[1][1] = i;
		m[2][2] = i;
	}

	friend std::ostream& operator<<(std::ostream& os, Mat3& m);
	friend std::ostream& operator<<(std::ostream& os, Mat3&& m);

	float* operator[](const int i);
	const float* operator[](const int i) const;
	Mat3 operator/(const float s);
	Mat3 operator*(const float s);
	Mat3 operator*(const Mat3& A) const;
	Mat3 operator*(const Mat3&& A);
	Vec3 operator*(Vec3& v);

	constexpr float determinant() const{
		float r = 
			m[0][0] * m[1][1] * m[2][2] +
			m[0][1] * m[1][2] * m[2][0] +
			m[0][2] * m[1][0] * m[2][1] -
			m[0][2] * m[1][1] * m[2][0] -
			m[0][1] * m[1][0] * m[2][2] -
			m[0][0] * m[1][2] * m[2][1];
		return r;
	}

	Mat3 cofactor() const;

	Mat3 inverse() const {
		if (rotation) return transpose();
		return (cofactor().transpose()) * (1.f / determinant());
	}

	Mat3 transpose() const{
		Mat3 t = (*this);
		std::swap(t.m[0][1], t.m[1][0]);
		std::swap(t.m[0][2], t.m[2][0]);
		std::swap(t.m[1][2], t.m[2][1]);
		return t;
	}

	static Mat3 rotate(Vec3& direction);
	static Mat3 rotate(Vec3&& direction);

	bool rotation = false;
	float m[3][3];

};

