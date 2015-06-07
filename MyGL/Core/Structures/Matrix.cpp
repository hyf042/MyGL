#include "Matrix.h"

namespace MyGL {
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
}