#pragma once
#include <iostream>

class Vec4 {
public:
	constexpr Vec4() : x(0), y(0), z(0), w(0) {}
	constexpr Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w){}
	constexpr Vec4(float v[4]) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

	friend std::ostream& operator<<(std::ostream& os, const Vec4& vector)  {
		return os << "(" << vector.x << ", " << vector.y << ", " << vector.z << ", " << vector.w << ")";  
	}

	constexpr Vec4 operator-(const Vec4& other) const {
		return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	constexpr static float dot(const Vec4& left, const Vec4& right) {
		return left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
	}

	constexpr float dot(const Vec4& other) const {
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	constexpr float operator[](const int i) {
		return raw[i];
	}

	constexpr float operator[](const int i) const {
		return raw[i];
	}

	float magnitude() const {
		return sqrtf((*this).dot((*this)));
	}
	union
	{
		struct{ float x, y, z, w; };
		float raw[4];
	};
	
};

constexpr Vec4 operator*(const float left, const Vec4& vector) {
	return Vec4(left * vector.x, left * vector.y, left * vector.z, left * vector.w);
}
constexpr Vec4 operator*(const Vec4& vector, const float right) {
	return Vec4(vector.x * right,vector.y * right,vector.z * right,vector.w * right);
}

constexpr Vec4 operator/(const float left, const Vec4& vector) {
	return Vec4(left / vector.x, left / vector.y, left / vector.z, left / vector.w);
}
constexpr Vec4 operator/(const Vec4& vector, const float right) {
	return Vec4(vector.x / right, vector.y / right, vector.z / right, vector.w / right);
}

