#ifndef _MYGL_CORE_STRUCTURES_MATRIX_H_
#define _MYGL_CORE_STRUCTURES_MATRIX_H_

#include "Vector.h"

namespace MyGL {
	template<int R, int C>
	class Matrix;
	class Matrix3x3;
	class Matrix4x4;
	class Matrix3x4;
	class Matrix4x3;

	template<int R, int C>
	class MatrixTypeTraits {
	public:
		typedef Matrix<R, C> matrix_type;
	};
	template<>
	class MatrixTypeTraits<4, 4> {
	public:
		typedef Matrix4x4 matrix_type;
	};
	template<>
	class MatrixTypeTraits<3, 3> {
	public:
		typedef Matrix3x3 matrix_type;
	};
	template<>
	class MatrixTypeTraits<3, 4> {
	public:
		typedef Matrix3x4 matrix_type;
	};
	template<>
	class MatrixTypeTraits<4, 3> {
	public:
		typedef Matrix4x3 matrix_type;
	};

	template<int ROW_NUM, int COL_NUM>
	class Matrix {
		typedef typename MatrixTypeTraits<ROW_NUM, COL_NUM>::matrix_type matrix_type;
		typedef Matrix<ROW_NUM, COL_NUM> self_type;
		typedef float row_array_type[COL_NUM];
	public:
		static const int kColNum = ROW_NUM;
		static const int kRowNum = COL_NUM;

		Matrix() {
			memset(&_values[0][0], 0, sizeof(_values));
		}
		Matrix(float value[kRowNum][kColNum]) {
			memcpy(&_values[0][0], &value[0][0], sizeof(_values));
		}
		virtual ~Matrix() = 0;
		matrix_type Clone() const;

		void SetValues(float value[][COL_NUM]) {
			memcpy(&_values[0][0], &value[0][0], sizeof(_values));
		}
		inline int GetRowSize() const { return COL_NUM; }
		inline int GetColSize() const { return ROW_NUM; }

		matrix_type Transpose() const;

		vector<float> GetRowArray(int row) const {
			vector<float> ret;
			for (int i = 0; i < COL_NUM; i++) {
				ret.push_back(_values[row][i]);
			}
			return ret;
		}
		vector<float> GetColArray(int col) const {
			vector<float> ret;
			for (int i = 0; i < ROW_NUM; i++) {
				ret.push_back(_values[i][col]);
			}
			return ret;
		}

		row_array_type& operator[](int index) {
			if (index < 0 || index >= kRowNum) {
				throw Exception::IndexOutOfRangeException();
			}
			return _values[index];
		}
		const row_array_type& operator[](int index) const {
			if (index < 0 || index >= kRowNum) {
				throw Exception::IndexOutOfRangeException();
			}
			return _values[index];
		}
		friend bool operator==(const Matrix &lhs, const Matrix &rhs) {
			for (int i = 0; i < Matrix::kRowNum; i++) {
				for (int j = 0; j < Matrix::kColNum; j++) {
					if (!Math::Equal(lhs[i][j], rhs[i][j])) {
						return false;
					}
				}
			}
			return true;
		}
		friend bool operator!=(const Matrix &lhs, const Matrix &rhs) {
			return !(lhs == rhs);
		}
		template<int T1, int T2, int T3>
		friend typename MatrixTypeTraits<T1, T3>::matrix_type operator*(const Matrix<T1, T2> &lhs, const Matrix<T2, T3> &rhs);

	private:
		float _values[ROW_NUM][COL_NUM];
	};

	class Matrix4x4 : public Matrix<4, 4> {
		typedef Matrix<4, 4> base_type;
	public:
		Matrix4x4() : base_type() {}
		Matrix4x4(const Matrix<4, 4> &other) : base_type(other) {}
		Matrix4x4(
				float v00, float v01, float v02, float v03,
				float v10, float v11, float v12, float v13,
				float v20, float v21, float v22, float v23,
				float v30, float v31, float v32, float v33) {
			float values[][4] = { { v00, v01, v02, v03 },
					{ v10, v11, v12, v13 },
					{ v20, v21, v22, v23 },
					{ v30, v31, v32, v33 } };
			SetValues(values);
		}
		override ~Matrix4x4() {}

		Vector4 GetRowVector(int row) const {
			auto rowData = GetRowArray(row);
			return Vector4(rowData[0], rowData[1], rowData[2], rowData[3]);
		}
		Vector4 GetColVector(int col) const {
			auto colData = GetColArray(col);
			return Vector4(colData[0], colData[1], colData[2], colData[3]);
		}
	};

	class Matrix3x3 : public Matrix<3, 3> {
		typedef Matrix<3, 3> base_type;
	public:
		Matrix3x3() : base_type() {}
		Matrix3x3(const Matrix<3, 3> &other) : base_type(other) {}
		Matrix3x3(
				float v00, float v01, float v02,
				float v10, float v11, float v12,
				float v20, float v21, float v22) {
			float values[][3] = { { v00, v01, v02 },
					{ v10, v11, v12},
					{ v20, v21, v22} };
			SetValues(values);
		}
		override ~Matrix3x3() {}

		Vector3 GetRowVector(int row) const {
			auto rowData = GetRowArray(row);
			return Vector3(rowData[0], rowData[1], rowData[2]);
		}
		Vector3 GetColVector(int col) const {
			auto colData = GetColArray(col);
			return Vector3(colData[0], colData[1], colData[2]);
		}
	};

	class Matrix3x4 : public Matrix<3, 4> {
		typedef Matrix<3, 4> base_type;
	public:
		Matrix3x4() : base_type() {}
		Matrix3x4(const Matrix<3, 4> &other) : base_type(other) {}
		Matrix3x4(
				float v00, float v01, float v02, float v03,
				float v10, float v11, float v12, float v13,
				float v20, float v21, float v22, float v23) {
			float values[][4] = { { v00, v01, v02, v03 },
					{ v10, v11, v12, v13 },
					{ v20, v21, v22, v23 } };
			SetValues(values);
		}
		override ~Matrix3x4() {}

		Vector4 GetRowVector(int row) const {
			auto rowData = GetRowArray(row);
			return Vector4(rowData[0], rowData[1], rowData[2], rowData[3]);
		}
		Vector3 GetColVector(int col) const {
			auto colData = GetColArray(col);
			return Vector3(colData[0], colData[1], colData[2]);
		}
	};

	class Matrix4x3 : public Matrix<4, 3> {
		typedef Matrix<4, 3> base_type;
	public:
		Matrix4x3() : base_type() {}
		Matrix4x3(const Matrix<4, 3> &other) : base_type(other) {}
		Matrix4x3(
			float v00, float v01, float v02,
			float v10, float v11, float v12,
			float v20, float v21, float v22,
			float v30, float v31, float v32) {
			float values[][3] = { { v00, v01, v02 },
					{ v10, v11, v12 },
					{ v20, v21, v22 },
					{ v30, v31, v32}};
			SetValues(values);
		}
		override ~Matrix4x3() {}

		Vector3 GetRowVector(int row) const {
			auto rowData = GetRowArray(row);
			return Vector3(rowData[0], rowData[1], rowData[2]);
		}
		Vector4 GetColVector(int col) const {
			auto colData = GetColArray(col);
			return Vector4(colData[0], colData[1], colData[2], colData[3]);
		}
	};

	template<int T1, int T2, int T3>
	typename MatrixTypeTraits<T1, T3>::matrix_type operator*(const Matrix<T1, T2> &lhs, const Matrix<T2, T3> &rhs) {
		typename MatrixTypeTraits<T1, T3>::matrix_type matrix;
		for (int i = 0; i < T1; i++) {
			for (int j = 0; j < T3; j++) {
				for (int k = 0; k < T2; k++) {
					matrix[i][j] = matrix[i][j] + lhs[i][k] * rhs[k][j];
				}
			}
		}
		return matrix;
	}
	template<int R, int C>
	typename Vector<R>::vector_type operator*(const Matrix<R, C> &lhs, typename Vector<C>::vector_type &rhs) {
		typename Vector<R>::vector_type result;
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				result[i] += lhs[i][j] * rhs[j];
			}
		}
		return result;
	}

	typedef Matrix4x4 Matrix4;
	typedef Matrix3x3 Matrix3;

	namespace Matrixs {
		Matrix4 Translate(float x, float y, float z);
		Matrix4 Scale(float x, float y, float z);
		Matrix4 Scale(float scale);
	}
}

#endif