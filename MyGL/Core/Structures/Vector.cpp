#include "Vector.h"

namespace MyGL {
	Vector3 Vector4::AsVector3() const {
		float _w = w();
		if (Math::IsZero(_w)) {
			return Vector3(x(), y(), z());
		}
		else {
			return Vector3(x() / _w, y() / _w, z() / _w);
		}
	}
	Vector4::operator Vector3() {
		return  AsVector3();
	}
	Vector4 Vector4::Of(const Vector3 &v3, bool isVector) {
		return Vector4(v3.x(), v3.y(), v3.z(), isVector ? 0.0f : 1.0f);
	}
}