#include "Vector.h"

namespace MyGL {
	template<int SIZE>
	typename Vector<SIZE>::vector_type Vector<SIZE>::Normalized() const {
		auto normalized = Clone();
		normalized.Normalize();
		return normalized;
	}

	Vector3 Vector4::AsVector3() {
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
	Vector4 Vector4::Of(const Vector3 &v3) {
		return Vector4(v3.x(), v3.y(), v3.z(), 1.0f);
	}
}