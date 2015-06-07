#ifndef _MYGL_CORE_VECTOR_H_
#define _MYGL_CORE_VECTOR_H_

#include <cmath>
#include "../PreDefines.h"
#include "../Exception.h"
#include "../Math.h"

namespace MyGL {
	template<int SIZE>
	class Vector;
	class Vector2;
	class Vector3;
	class Vector4;

	template<int SIZE>
	class VectorTypeTraits {
	public:
		typedef Vector<SIZE> vector_type;
	};
	template<>
	class VectorTypeTraits<2> {
	public:
		typedef Vector2 vector_type;
	};
	template<>
	class VectorTypeTraits<3> {
	public:
		typedef Vector3 vector_type;
	};
	template<>
	class VectorTypeTraits<4> {
	public:
		typedef Vector4 vector_type;
	};

	template<int SIZE>
	class Vector {
	public:
		typedef typename VectorTypeTraits<SIZE>::vector_type vector_type;

		static const int kSize = SIZE;

		vector_type Clone() const;
		inline float LengthSquare() const {
			float sum = 0;
			for (int i = 0; i < SIZE; i++) {
				sum += _values[i] * _values[i];
			}
			return sum;
		}
		inline float Length() const {
			return sqrt(LengthSquare());
		}
		inline void Normalize() {
			float length = Length();
			if (Math::IsZero(length)) {
				return;
			}
			for (int i = 0; i < SIZE; i++) {
				_values[i] /= length;
			}
		}
		vector_type Normalized() const;

		float& operator[](int index) {
			if (index < 0 || index >= kSize) {
				throw Exception::IndexOutOfRangeException();
			}
			return _values[index];
		}
		float operator[](int index) const {
			if (index < 0 || index >= kSize) {
				throw Exception::IndexOutOfRangeException();
			}
			return _values[index];
		}
		Vector& operator+=(const Vector &other) {
			for (int i = 0; i < SIZE; i++) {
				_values[i] += other._values[i];
			}
			return *this;
		}
		Vector& operator-=(const Vector &other) {
			for (int i = 0; i < SIZE; i++) {
				_values[i] -= other._values[i];
			}
			return *this;
		}
		Vector& operator*=(float scale) {
			for (int i = 0; i < SIZE; i++) {
				_values[i] *= scale;
			}
			return *this;
		}
		friend float Dot(const Vector &lhs, const Vector &rhs) {
			float sum = 0;
			for (int i = 0; i < SIZE; i++) {
				sum = lhs._values[i] * rhs._values[i];
			}
			return sum;
		}
		friend bool operator==(const Vector &lhs, const Vector &rhs) {
			for (int i = 0; i < SIZE; i++) {
				if (!Math::Equal(lhs._values[i], rhs._values[i])) {
					return false;
				}
			}
			return true;
		}
		friend bool operator!=(const Vector &lhs, const Vector &rhs) {
			return !(lhs == rhs);
		}

		vector_type Zero();

	protected:
		Vector() {
			memset(_values, 0, sizeof(_values));
		}
		Vector(float values[SIZE]) {
			memcpy(_values, values, sizeof(_values));
		}
		template<int OTHER_SIZE>
		Vector(Vector<OTHER_SIZE> other) {
			int size = Math::Min(SIZE, OTHER_SIZE);
			for (int i = 0; i < size; i++) {
				_values[i] = other._values[i];
			}
			for (int i = size; i < SIZE; i++) {
				_values[i] = 0;
			}
		}

		float _values[SIZE];
	};

	class Vector4 : public Vector<4> {
		typedef Vector<4> base_type;

	public:
		Vector4() : base_type() {}
		Vector4(float x, float y, float z) {
			SetValue(x, y, z, 1.0f);
		}
		Vector4(float x, float y, float z, float w) {
			SetValue(x, y, z, w);
		}
		template<int OTHER_SIZE>
		Vector4(const Vector<OTHER_SIZE> &other) : base_type(other) {}
		template<int OTHER_SIZE>
		static Vector4 Of(const Vector<OTHER_SIZE> &other) { return Vector4(other); }
		// Specialize for v3 -> v4, w = 1
		static Vector4 Of(Vector3 &v3);
		Vector3 AsVector3();

		void SetValue(float x, float y, float z, float w) {
			_values[0] = x;
			_values[1] = y;
			_values[2] = z;
			_values[3] = w;
		}

		inline float x() const {
			return _values[0];
		}
		inline float y() const {
			return _values[1];
		}
		inline float z() const {
			return _values[2];
		}
		inline float w() const {
			return _values[3];
		}
		inline void set_x(float x) {
			_values[0] = x;
		}
		inline void set_y(float y) {
			_values[1] = y;
		}
		inline void set_z(float z) {
			_values[2] = z;
		}
		inline void set_w(float w) {
			_values[3] = w;
		}
	};

	class Vector3 : public Vector<3> {
		typedef Vector<3> base_type;

	public:
		Vector3() : base_type() {}
		Vector3(float x, float y, float z) {
			SetValue(x, y, z);
		}
		template<int OTHER_SIZE>
		Vector3(const Vector<OTHER_SIZE> &other) : base_type(other) {}
		template<int OTHER_SIZE>
		static Vector3 Of(const Vector<OTHER_SIZE> &other) { return Vector3(other); }

		void SetValue(float x, float y, float z) {
			_values[0] = x;
			_values[1] = y;
			_values[2] = z;
		}

		friend Vector3 Cross(const Vector3 &lhs, const Vector3 &rhs) {
			return Vector3(lhs.y() * rhs.z() - lhs.z() * rhs.y(), lhs.z() * rhs.x() - lhs.x() * rhs.z(), lhs.x() * rhs.y() - lhs.y() * rhs.x());
		}

		inline float x() const {
			return _values[0];
		}
		inline float y() const {
			return _values[1];
		}
		inline float z() const {
			return _values[2];
		}
		inline void set_x(float x) {
			_values[0] = x;
		}
		inline void set_y(float y) {
			_values[1] = y;
		}
		inline void set_z(float z) {
			_values[2] = z;
		}
	};

	class Vector2 : public Vector<2> {
		typedef Vector<2> base_type;

	public:
		Vector2() : base_type() {}
		Vector2(float x, float y) {
			SetValue(x, y);
		}
		template<int OTHER_SIZE>
		Vector2(const Vector<OTHER_SIZE> &other) : base_type(other) {}
		template<int OTHER_SIZE>
		static Vector2 Of(const Vector<OTHER_SIZE> &other) { return Vector2(other); }

		void SetValue(float x, float y) {
			_values[0] = x;
			_values[1] = y;
		}

		friend float Cross(const Vector2 &lhs, const Vector2 &rhs) {
			return lhs.x() * rhs.y() - lhs.y() * rhs.x();
		}

		inline float x() const {
			return _values[0];
		}
		inline float y() const {
			return _values[1];
		}
		inline void set_x(float x) {
			_values[0] = x;
		}
		inline void set_y(float y) {
			_values[1] = y;
		}
	};

	template<int SIZE>
	typename VectorTypeTraits<SIZE>::vector_type operator+(const Vector<SIZE> &lhs, const Vector<SIZE> &rhs) {
		typename VectorTypeTraits<SIZE>::vector_type ret =lhs;
		ret += rhs;
		return ret;
	}
	template<int SIZE>
	typename VectorTypeTraits<SIZE>::vector_type operator-(const Vector<SIZE> &lhs, const Vector<SIZE> &rhs) {
		typename VectorTypeTraits<SIZE>::vector_type ret = lhs;
		ret -= rhs;
		return ret;
	}
	template<int SIZE>
	typename VectorTypeTraits<SIZE>::vector_type operator*(const Vector<SIZE> &lhs, float rhs) {
		typename VectorTypeTraits<SIZE>::vector_type ret = lhs;
		ret *= rhs;
		return ret;
	}
	template<int SIZE>
	typename VectorTypeTraits<SIZE>::vector_type operator*(float lhs, const Vector<SIZE> &rhs) {
		typename VectorTypeTraits<SIZE>::vector_type ret = rhs;
		ret *= lhs;
		return ret;
	}
	template<int SIZE>
	typename VectorTypeTraits<SIZE>::vector_type Vector<SIZE>::Zero() {
		return typename VectorTypeTraits<SIZE>::vector_type();
	}
}

#endif