#include "Matrix4.h"
#include "Matrix3.h"
#include "Vector3.h"
#include "Vector4.h"

Mat4::Mat4(Vec4 v, Vec4 u, Vec4 w, Vec4 r) : 
	m{
		{v.raw[0], v.raw[1], v.raw[2], v.raw[3]},
		{u.raw[0], u.raw[1], u.raw[2], u.raw[3]},
		{w.raw[0], w.raw[1], w.raw[2], w.raw[3]},
		{r.raw[0], r.raw[1], r.raw[2], r.raw[3]} } {}
Mat4::Mat4(float i) : Mat4() { m[0][0] = i; m[1][1] = i; m[2][2] = i; m[3][3] = i; }

Mat4::Mat4(Mat3& matrix) {
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			m[i][j] = matrix[i][j];
		}
	}
	m[0][3] = 0;
	m[1][3] = 0;
	m[2][3] = 0;
	m[3][3] = 1;
}

float Mat4::determinant() const {
	Mat3 A({ m[1][1], m[1][2], m[1][3] }, { m[2][1], m[2][2], m[2][3] }, { m[3][1], m[3][2], m[3][3] });
	Mat3 B({ m[1][0], m[1][2], m[1][3] }, { m[2][0], m[2][2], m[2][3] }, { m[3][0], m[3][2], m[3][3] });
	Mat3 C({ m[1][0], m[1][1], m[1][3] }, { m[2][0], m[2][1], m[2][3] }, { m[3][0], m[3][1], m[3][3] });
	Mat3 D({ m[1][0], m[1][1], m[1][2] }, { m[2][0], m[2][1], m[2][2] }, { m[3][0], m[3][1], m[3][2] });

	float a = m[0][0] * A.determinant();
	float b = m[0][1] * B.determinant();
	float c = m[0][2] * C.determinant();
	float d = m[0][3] * D.determinant();

	return a - b + c - d;
}

/*
	C_(ij) = (-1) ^ i * j
	
*/

Mat4 Mat4::cofactor() const {
	Mat3 A({ m[1][1], m[1][2], m[1][3] }, { m[2][1], m[2][2], m[2][3] }, { m[3][1], m[3][2], m[3][3] });
	Mat3 B({ m[1][0], m[1][2], m[1][3] }, { m[2][0], m[2][2], m[2][3] }, { m[3][0], m[3][2], m[3][3] });
	Mat3 C({ m[1][0], m[1][1], m[1][3] }, { m[2][0], m[2][1], m[2][3] }, { m[3][0], m[3][1], m[3][3] });
	Mat3 D({ m[1][0], m[1][1], m[1][2] }, { m[2][0], m[2][1], m[2][2] }, { m[3][0], m[3][1], m[3][2] });

	Mat3 E({ m[0][1], m[0][2], m[0][3] }, { m[2][1], m[2][2], m[2][3] }, { m[3][1], m[3][2], m[3][3] });
	Mat3 F({ m[0][0], m[0][2], m[0][3] }, { m[2][0], m[2][2], m[2][3] }, { m[3][0], m[3][2], m[3][3] });
	Mat3 G({ m[0][0], m[0][1], m[0][3] }, { m[2][0], m[2][1], m[2][3] }, { m[3][0], m[3][1], m[3][3] });
	Mat3 H({ m[0][0], m[0][1], m[0][2] }, { m[2][0], m[2][1], m[2][2] }, { m[3][0], m[3][1], m[3][2] });

	Mat3 I({ m[0][1], m[0][2], m[0][3] }, { m[1][1], m[1][2], m[1][3] }, { m[3][1], m[3][2], m[3][3] });
	Mat3 J({ m[0][0], m[0][2], m[0][3] }, { m[1][0], m[1][2], m[1][3] }, { m[3][0], m[3][2], m[3][3] });
	Mat3 K({ m[0][0], m[0][1], m[0][3] }, { m[1][0], m[1][1], m[1][3] }, { m[3][0], m[3][1], m[3][3] });
	Mat3 L({ m[0][0], m[0][1], m[0][2] }, { m[1][0], m[1][1], m[1][2] }, { m[3][0], m[3][1], m[3][2] });

	Mat3 M({ m[0][1], m[0][2], m[0][3] }, { m[1][1], m[1][2], m[1][3] }, { m[2][1], m[2][2], m[2][3] });
	Mat3 N({ m[0][0], m[0][2], m[0][3] }, { m[1][0], m[1][2], m[1][3] }, { m[2][0], m[2][2], m[2][3] });
	Mat3 O({ m[0][0], m[0][1], m[0][3] }, { m[1][0], m[1][1], m[1][3] }, { m[2][0], m[2][1], m[2][3] });
	Mat3 P({ m[0][0], m[0][1], m[0][2] }, { m[1][0], m[1][1], m[1][2] }, { m[2][0], m[2][1], m[2][2] });

	Vec4 v(A.determinant(), -B.determinant(), C.determinant(), -D.determinant());
	Vec4 u(-E.determinant(), F.determinant(), -G.determinant(), H.determinant());
	Vec4 w(I.determinant(),-J.determinant(), K.determinant(), -L.determinant());
	Vec4 z(-M.determinant(), N.determinant(), -O.determinant(), P.determinant());
	Mat4 result(v, u, w, z);

	return result;
}

Mat4 Mat4::transpose() {
	Mat4 t = (*this);
	std::swap(t.m[0][1], t.m[1][0]);
	std::swap(t.m[0][2], t.m[2][0]);
	std::swap(t.m[1][3], t.m[3][1]);
	std::swap(t.m[1][2], t.m[2][1]);
	std::swap(t.m[0][3], t.m[3][0]);
	std::swap(t.m[2][3], t.m[3][2]);
	return t;
}

Mat4 Mat4::inverse() const {
	float det = determinant();
	Mat4 co = cofactor().transpose();
	if (det == 0) return co;
	return co * (1.f / det);
}

Mat4 Mat4::perspective(float width, float height, float near, float far, float fov) {
	float right = near * std::tanf(fov / 2.f) * (width / height);
	float top = near * std::tanf(fov / 2.f);
	float A = (far + near) / (far - near);
	float B = (-2 * far * near) / (far - near);

	return Mat4(
		{ near / right, 0, 0, 0 }, 
		{ 0, near / top, 0, 0 }, 
		{ 0, 0, A, B }, 
		{ 0, 0, 1, 0 });
}

Vec3 Mat4::translate(Vec3 position, Vec3 original) {
	Mat4 T({ 1, 0, 0, position[0] }, { 0, 1, 0, position[1] }, { 0, 0, 1, position[3] }, { 0, 0, 0, 1 });
	Vec4 r = T * original;
	return r;
}

Mat4 Mat4::operator*(const Mat4& A) {
	float a = m[0][0] * A[0][0] + m[0][1] * A[1][0] + m[0][2] * A[2][0] + m[0][3] * A[3][0];
	float b = m[0][0] * A[0][1] + m[0][1] * A[1][1] + m[0][2] * A[2][1] + m[0][3] * A[3][1];
	float c = m[0][0] * A[0][2] + m[0][1] * A[1][2] + m[0][2] * A[2][2] + m[0][3] * A[3][2];
	float d = m[0][0] * A[0][3] + m[0][1] * A[1][3] + m[0][2] * A[2][3] + m[0][3] * A[3][3];
	Vec4 v(a, b, c, d);

	float e = m[1][0] * A[0][0] + m[1][1] * A[1][0] + m[1][2] * A[2][0] + m[1][3] * A[3][0];
	float f = m[1][0] * A[0][1] + m[1][1] * A[1][1] + m[1][2] * A[2][1] + m[1][3] * A[3][1];
	float g = m[1][0] * A[0][2] + m[1][1] * A[1][2] + m[1][2] * A[2][2] + m[1][3] * A[3][2];
	float h = m[1][0] * A[0][3] + m[1][1] * A[1][3] + m[1][2] * A[2][3] + m[1][3] * A[3][3];
	Vec4 u(e, f, g, h);

	float i = m[2][0] * A[0][0] + m[2][1] * A[1][0] + m[2][2] * A[2][0] + m[2][3] * A[3][0];
	float j = m[2][0] * A[0][1] + m[2][1] * A[1][1] + m[2][2] * A[2][1] + m[2][3] * A[3][1];
	float k = m[2][0] * A[0][2] + m[2][1] * A[1][2] + m[2][2] * A[2][2] + m[2][3] * A[3][2];
	float l = m[2][0] * A[0][3] + m[2][1] * A[1][3] + m[2][2] * A[2][3] + m[2][3] * A[3][3];
	Vec4 w(i, j, k, l);

	float mm = m[3][0] * A[0][0] + m[3][1] * A[1][0] + m[3][2] * A[2][0] + m[3][3] * A[3][0];
	float n = m[3][0] * A[0][1] + m[3][1] * A[1][1] + m[3][2] * A[2][1] + m[3][3] * A[3][1];
	float o = m[3][0] * A[0][2] + m[3][1] * A[1][2] + m[3][2] * A[2][2] + m[3][3] * A[3][2];
	float p = m[3][0] * A[0][3] + m[3][1] * A[1][3] + m[3][2] * A[2][3] + m[3][3] * A[3][3];
	Vec4 z(mm, n, o, p);

	return Mat4(v, u, w, z);
}

Mat4 Mat4::operator*(const Mat4&& A) {
	float a = m[0][0] * A[0][0] + m[0][1] * A[1][0] + m[0][2] * A[2][0] + m[0][3] * A[3][0];
	float b = m[0][0] * A[0][1] + m[0][1] * A[1][1] + m[0][2] * A[2][1] + m[0][3] * A[3][1];
	float c = m[0][0] * A[0][2] + m[0][1] * A[1][2] + m[0][2] * A[2][2] + m[0][3] * A[3][2];
	float d = m[0][0] * A[0][3] + m[0][1] * A[1][3] + m[0][2] * A[2][3] + m[0][3] * A[3][3];
	Vec4 v(a, b, c, d);

	float e = m[1][0] * A[0][0] + m[1][1] * A[1][0] + m[1][2] * A[2][0] + m[1][3] * A[3][0];
	float f = m[1][0] * A[0][1] + m[1][1] * A[1][1] + m[1][2] * A[2][1] + m[1][3] * A[3][1];
	float g = m[1][0] * A[0][2] + m[1][1] * A[1][2] + m[1][2] * A[2][2] + m[1][3] * A[3][2];
	float h = m[1][0] * A[0][3] + m[1][1] * A[1][3] + m[1][2] * A[2][3] + m[1][3] * A[3][3];
	Vec4 u(e, f, g, h);

	float i = m[2][0] * A[0][0] + m[2][1] * A[1][0] + m[2][2] * A[2][0] + m[2][3] * A[3][0];
	float j = m[2][0] * A[0][1] + m[2][1] * A[1][1] + m[2][2] * A[2][1] + m[2][3] * A[3][1];
	float k = m[2][0] * A[0][2] + m[2][1] * A[1][2] + m[2][2] * A[2][2] + m[2][3] * A[3][2];
	float l = m[2][0] * A[0][3] + m[2][1] * A[1][3] + m[2][2] * A[2][3] + m[2][3] * A[3][3];
	Vec4 w(i, j, k, l);

	float mm = m[3][0] * A[0][0] + m[3][1] * A[1][0] + m[3][2] * A[2][0] + m[3][3] * A[3][0];
	float n = m[3][0] * A[0][1] + m[3][1] * A[1][1] + m[3][2] * A[2][1] + m[3][3] * A[3][1];
	float o = m[3][0] * A[0][2] + m[3][1] * A[1][2] + m[3][2] * A[2][2] + m[3][3] * A[3][2];
	float p = m[3][0] * A[0][3] + m[3][1] * A[1][3] + m[3][2] * A[2][3] + m[3][3] * A[3][3];
	Vec4 z(mm, n, o, p);

	return Mat4(v, u, w, z);
}

Mat4 Mat4::operator/(const float f) {
	Vec4 v = m[0];
	Vec4 u = m[1];
	Vec4 w = m[2];
	Vec4 z = m[3];

	return Mat4(v / f, u / f, w / f, z / f);
}

Mat4 Mat4::operator*(const float f) {
	Vec4 v = m[0];
	Vec4 u = m[1];
	Vec4 w = m[2];
	Vec4 z = m[3];

	return Mat4(v * f, u * f, w * f, z * f);
}

Vec4 Mat4::operator*(const Vec4& v) {
	Vec4 u;
	u.x = m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2] + m[0][3] * v[3];
	u.y = m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2] + m[1][3] * v[3];
	u.z = m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2] + m[2][3] * v[3];
	u.w = 1;
	return u;
}

Vec4 Mat4::operator*(const Vec3& v) {
	Vec4 u(v[0], v[1], v[2], 1);
	return m * u;
}

