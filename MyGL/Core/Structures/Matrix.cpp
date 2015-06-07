#include "Matrix.h"

namespace MyGL {
	template<int R, int C>
	Matrix<R, C>::~Matrix() {}

	template<int R, int C>
	typename Matrix<R, C>::matrix_type Matrix<R,C>::Clone() const {
		return typename Matrix<R, C>::matrix_type(*this);
	}

	template<int R, int C>
	typename Matrix<R, C>::matrix_type Matrix<R, C>::Transpose() const {
		typename Matrix<R, C>::matrix_type matrix;
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				matrix[j][i] = _values[i][j];
			}
		}
		return matrix;
	}

	namespace Matrixs {
		Matrix4 Translate(float x, float y, float z) {
			return Matrix4(
					1, 0, 0, x,
					0, 1, 0, y,
					0, 0, 1, z,
					0, 0, 0, 1);
		}
		Matrix4 Scale(float x, float y, float z) {
			return Matrix4(
					x, 0, 0, 0,
					0, y, 0, 0,
					0, 0, z, 0,
					0, 0, 0, 1);
		}
		Matrix4 Scale(float scale) { return Scale(scale, scale, scale); }
	}
}