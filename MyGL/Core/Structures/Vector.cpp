#include "Vector.h"

namespace MyGL {
	template<int SIZE>
	typename Vector<SIZE>::vector_type Vector<SIZE>::Clone() const {
		return typename Vector<SIZE>::vector_type(*this);
	}
	template<int SIZE>
	typename Vector<SIZE>::vector_type Vector<SIZE>::Normalized() const {
		typename Vector<SIZE>::vector_type normalized = Clone();
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
	Vector4 Vector4::OfPoint(Vector3 &v3) {
		return Vector4(v3.x(), v3.y(), v3.z(), 1.0f);
	}
}