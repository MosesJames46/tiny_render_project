#include "Vector3.h"
#include "Vector4.h"

Vec3::Vec3(Vec4& v) {
	for (int i = 0; i < 3; ++i) {
		raw_data[i] = v[i];
	}
}