#include "Matrix3.h"
#include "Vector3.h"

Mat3::Mat3(Vec3 x, Vec3 y, Vec3 z){
	for (int i = 0; i < 3; ++i) {
		m[0][i] = x[i];
		m[1][i] = y[i];
		m[2][i] = z[i];
	}
}

std::ostream& operator<<(std::ostream& os, Mat3& m) {
	for (int i = 0; i < 3; ++i) {
		os << "[";
		for (int j = 0; j < 3; ++j) {
			os << m.m[i][j] << " ";
		}
		os << "\b]";
		os << std::endl;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, Mat3&& m) {
	for (int i = 0; i < 3; ++i) {
		os << "[";
		for (int j = 0; j < 3; ++j) {
			os << m.m[i][j] << " ";
		}
		os << "\b]";
		os << std::endl;
	}
	return os;
}


float* Mat3::operator[](const int i){
	return m[i];
}

const float* Mat3::operator[](const int i) const{
	return m[i];
}

Mat3 Mat3::cofactor() const {
	float A = m[1][1] * m[2][2] - m[1][2] * m[2][1];
	float B = -(m[1][0] * m[2][2] - m[1][2] * m[2][0]);
	float C = m[1][0] * m[2][1] - m[1][1] * m[2][0];
	float D = -(m[0][1] * m[2][2] - m[0][2] * m[2][0]);
	float E = m[0][0] * m[2][2] - m[0][2] * m[2][0];
	float F = -(m[0][0] * m[2][1] - m[0][1] * m[2][0]);
	float G = m[0][1] * m[1][2] - m[1][1] * m[0][2];
	float H = -(m[0][0] * m[1][2] - m[0][2] * m[1][0]);
	float I = m[0][0] * m[1][1] - m[0][1] * m[1][0];

	return Mat3({ A, B, C }, { D, E, F }, { G, H, I });
}

Mat3 Mat3::rotate(Vec3& v) {
	float pi = 3.141592f / 180.f;
	float a = v.z * pi;
	float b = v.y * pi;
	float l = v.x * pi;
	Mat3 Rx(
		{ 1, 0, 0 }, 
		{ 0, std::cosf(l), -std::sinf(l) }, 
		{ 0, std::sinf(l), std::cosf(l) });

	Mat3 Ry(
		{ std::cosf(b), 0, std::sinf(b) }, 
		{ 0, 1, 0 }, 
		{ -std::sinf(b), 0, std::cosf(b) });

	Mat3 Rz(
		{ std::cosf(a), -std::sinf(a), 0 }, 
		{ std::sinf(a), std::cosf(a), 0 }, 
		{ 0, 0, 1 });

	Mat3 R = Rx * Ry * Rz;
	return R;
}

Mat3 Mat3::rotate(Vec3&& v) {
	float pi = 3.141592f / 180.f;
	float a = v.z * pi;
	float b = v.y * pi;
	float l = v.x * pi;

	Mat3 Rx({ 1, 0, 0 }, { 0, std::cosf(l), -std::sinf(l) }, { 0, std::sinf(l), std::cosf(l) });
	Mat3 Ry({ std::cosf(b), 0, std::sinf(b) }, { 0, 1, 0 }, { -std::sinf(b), 0, std::cosf(b) });
	Mat3 Rz({ std::cosf(a), -std::cosf(a), 0 }, { std::sinf(a), std::cosf(a), 0 }, { 0, 0, 1 });

	Mat3 R = Rx * Ry * Rz;
	return R;
}

Mat3 Mat3::operator/(const float s) {
	Vec3 v = m[0];
	Vec3 u = m[1];
	Vec3 w = m[2];

	return Mat3(v / s, u / s, w / s);
}

Mat3 Mat3::operator*(const float s) {
	Vec3 v = m[0];
	Vec3 u = m[1];
	Vec3 w = m[2];

	return Mat3(v * s, u * s, w * s);
}

Mat3 Mat3::operator*(const Mat3& A) const{
	float a = m[0][0] * A[0][0] + m[0][1] * A[1][0] + m[0][2] * A[2][0];
	float b = m[0][0] * A[0][1] + m[0][1] * A[1][1] + m[0][2] * A[2][1];
	float c = m[0][0] * A[0][2] + m[0][1] * A[1][2] + m[0][2] * A[2][2];

	float d = m[1][0] * A[0][0] + m[1][1] * A[1][0] + m[1][2] * A[2][0];
	float e = m[1][0] * A[0][1] + m[1][1] * A[1][1] + m[1][2] * A[2][1];
	float f = m[1][0] * A[0][2] + m[1][1] * A[1][2] + m[1][2] * A[2][2];

	float g = m[2][0] * A[0][0] + m[2][1] * A[1][0] + m[2][2] * A[2][0];
	float h = m[2][0] * A[0][1] + m[2][1] * A[1][1] + m[2][2] * A[2][1];
	float i = m[2][0] * A[0][2] + m[2][1] * A[1][2] + m[2][2] * A[2][2];

	return Mat3({ a, b, c }, { d, e, f }, { g, h, i });
}

Mat3 Mat3::operator*(const Mat3&& A) {
	float a = m[0][0] * A[0][0] + m[0][1] * A[1][0] + m[0][2] * A[2][0];
	float b = m[0][0] * A[0][1] + m[0][1] * A[1][1] + m[0][2] * A[2][1];
	float c = m[0][0] * A[0][2] + m[0][1] * A[1][2] + m[0][2] * A[2][2];

	float d = m[1][0] * A[0][0] + m[1][1] * A[1][0] + m[1][2] * A[2][0];
	float e = m[1][0] * A[0][1] + m[1][1] * A[1][1] + m[1][2] * A[2][1];
	float f = m[1][0] * A[0][2] + m[1][1] * A[1][2] + m[1][2] * A[2][2];

	float g = m[2][0] * A[0][0] + m[2][1] * A[1][0] + m[2][2] * A[2][0];
	float h = m[2][0] * A[0][1] + m[2][1] * A[1][1] + m[2][2] * A[2][1];
	float i = m[2][0] * A[0][2] + m[2][1] * A[1][2] + m[2][2] * A[2][2];

	return Mat3({ a, b, c }, { d, e, f }, { g, h, i });
}

Vec3 Mat3::operator*(Vec3& v){
	Vec3 u;
	u.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
	u.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
	u.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
	return u;
}