#pragma once
#include <vector>
#include <iostream>

constexpr const float degree_conversion = 180.0f / 3.141592f;
class Vec4;

class Vec3 {
public:
	constexpr Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	constexpr Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	constexpr Vec3(float arr[3]) : x(arr[0]), y(arr[1]), z(arr[2]) {}
	Vec3(Vec4& v);

	friend std::ostream& operator<<(std::ostream& os, Vec3& vector) {
		return os << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
	}
	friend std::ostream& operator<<(std::ostream& os, Vec3&& vector) {
		return os << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
	}

	constexpr float operator[](const int i) const{
		return raw_data[i];
	}

	constexpr Vec3 operator*(float operand) const {
		return Vec3(this->x * operand, this->y * operand, this->z * operand);
	}

	constexpr Vec3 operator+(const Vec3& other) const{
		return Vec3(this->x + other.x, this->y + other.y, this->z + other.z);
	}



	constexpr Vec3 operator-(const Vec3& other) const {
		return Vec3(this->x - other.x, this->y - other.y, this->z - other.z);
	}

	constexpr Vec3 operator/(float operand) const {
		return Vec3(this->x / operand, this->y / operand, this->z / operand);
	}

	constexpr Vec3 operator/=(float operand) {
		this->x /= operand; 
		this->y /= operand; 
		this->z /= operand;
		return (*this);
	}

	float magnitude() const{
		return std::sqrtf((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
	}

	constexpr float square_magnitude() const {
		return (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
	}

	static float magnitude(Vec3 vector) {
		return sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	}

	static float square_magnitude(Vec3 vector) {
		return (vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z);
	}

	/*
		Recall that the dot product shows us how vector v aligns with vector u. Or the shadow of v casted on to u.
	*/
	constexpr float dot(const Vec3& other) const{
		return this->x * other.x + this->y * other.y + this->z * other.z;
	}

	static constexpr float dot(const Vec3& v, const Vec3& u) {
		return v.x * u.x + v.y * u.y + v.z * u.z;
	}

	Vec3 distance(const Vec3& other) const {
		return Vec3(this->x - other.x, this->y - other.y, this->z - other.z);
	}

	static constexpr Vec3 distance(const Vec3& v, const Vec3& u) {
		return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
	}

	constexpr Vec3 cross_product(Vec3& vector) const {
		/*
			Given that (i, j, k) are positively oriented orthonomal basis vectors the following equalities hold:
				1. i x j = k
				2. j x k = i
				3. k x i = j
			A mnemonic is that a cyclic permuation can be can be used to get from and one basis.
				i -> j -> k -> i
				- i x j = k -> i -> j.
				- j x k = i -> j -> k
				- k x i = j -> k -> i
				i.e. The ordering of the basis matters. The first basis should lead to the next in the proper order to yield a positive basis.
			With the hint above, the anticommuutativity should then be much easier. The result of anticummativity is negative basis.
				- i x k = -j
				- k x j = -i
				- j x i = -k

			These facts are necessary for the following expansion of the cross on vectors v and u.
			v = v1 * i + v2 * j + v3 * k
			u = u1 * i + u2 * j + u3 * k
				- This pretty much states that the inidvidual components times their respective basis vectors equals the original vector.

				We can then expand further with distributive property:
					- v x u = (v1 * i + v2 * j + v3 * k) x (u1 * i + u2 * j + u3 * k)
							= v1*u1(i x i) + v1*u2(i x j) + v1*u3(i x k)
							+ v2*u1(j x i) + v2*u2(j x j) + v2*u3(j x k)
							+ v3*u1(k x i) + v3*u2(k x j) + v3*u3(k x k)
				Given our mnemonic and the fact that a basis vector cross with itself is 0, we get:
					v x u = v1*u1(0) + v1*u2(0, 0, 1) + v1*u3(0, -1, 0)
							+ v2*u1(0, 0, -1) + v2*u2(0) + v2*u3(1, 0, 0)
							+ v3*u1(0, 1, 0) + v3*u2(-1, 0, 0) + v3*u3(0)
						  = (v1*u2 - v2*u1)i + (v3*u1 - v1*u3)j + (v2*u3 - v3*u2)k
				**Do note in the above, i, j, and k are simplified from their respective vectors. (0, 0, -1) could be written as -k.
				With this information, we know then that the cross product are scalars s1, s2, and s3 for their respective i, j, and k basis vectors.
				Since we are adding all the results, it is faster to calculate the values in place. 
		*/	
		return Vec3(this->y * vector.z - this->z * vector.y, 
			this->z * vector.x - this->x * vector.z, 
			this->x * vector.y - this->y * vector.x);
	}

	static constexpr Vec3 cross_product(Vec3 v, Vec3 u) {
		return Vec3(v.y * u.z - v.z * u.y, 
			v.z * u.x - v.x * u.z, 
			v.x * u.y - v.y * u.x);
	}

	Vec3 normalize() {
		float mag = magnitude();
		
		return (*this) / mag;
	}

	static Vec3 normalize(Vec3&& v) {
		return v / v.magnitude();
	}

	static Vec3 optimize_normalize(Vec3&& v) {
		float m = square_magnitude(v);
		if (m == 0) return v;

		float inv = 1 / sqrtf(m);
		return v * inv;
	}

	union {
		struct { float x, y, z; };
		float raw_data[3];
	};
private:
	float degrees_from_dot_over_mag(Vec3& vector) const {
		/*
			The degrees can be obtain from the dot product of vector u and v divided by the product of their magnitudes
		*/
		
		float degree_radians = std::acosf(this->dot(vector) / (this->magnitude() * vector.magnitude()));
		return degree_radians;
	}
	Vec3 unit_vector(Vec3& vector) const {
		Vec3 cross = cross_product(vector);
		return cross / (this->magnitude() * vector.magnitude() * std::sinf(degrees_from_dot_over_mag(vector)));
	}
};