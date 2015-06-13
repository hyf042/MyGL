#ifndef _MYGL_CORE_STRUCTURES_MATRIX_H_
#define _MYGL_CORE_STRUCTURES_MATRIX_H_

#include "Vector.h"
#include "../Interfaces//IClonable.h"

namespace MyGL {
	template<int ROW_NUM, int COL_NUM>
	class Matrix : public IStringify, public IClonable<Matrix<ROW_NUM, COL_NUM>> {
		typedef Matrix<ROW_NUM, COL_NUM> self_type;
		typedef float row_array_type[COL_NUM];
	public:
		static const int kColNum = ROW_NUM;
		static const int kRowNum = COL_NUM;

		Matrix() {
			memset(&_values[0][0], 0, sizeof(_values));
		}
		Matrix(float value[ROW_NUM][COL_NUM]) {
			memcpy(&_values[0][0], &value[0][0], sizeof(_values));
		}
		virtual ~Matrix() {}
		override self_type Clone() const {
			return self_type(*this);
		}
		override shared_ptr<Matrix> ClonePtr() const {
			return make_shared<Matrix>(*this);
		}
		override string ToString() const {
			std::stringstream ss;
			ss << "[";
			for (int i = 0; i < ROW_NUM; i++) {
				if (i != 0) {
					ss << " ";
				}
				bool flag = false;
				for (int j = 0; j < COL_NUM; j++) {
					if (flag) {
						ss << ", ";
					}
					else {
						flag = true;
					}
					ss << setiosflags(std::ios::fixed) << std::setprecision(3) << std::setw(10) << _values[i][j];
				}
				if (i < ROW_NUM - 1) {
					ss << std::endl;
				}
			}
			ss << "]";
			return ss.str();
		}

		void SetValues(float value[][COL_NUM]) {
			memcpy(&_values[0][0], &value[0][0], sizeof(_values));
		}
		inline int GetRowSize() const { return COL_NUM; }
		inline int GetColSize() const { return ROW_NUM; }

		Matrix<COL_NUM, ROW_NUM> Transpose() const {
			Matrix<COL_NUM, ROW_NUM> matrix;
			for (int i = 0; i < ROW_NUM; i++) {
				for (int j = 0; j < COL_NUM; j++) {
					matrix[j][i] = _values[i][j];
				}
			}
			return matrix;
		}

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

		typename VectorTypeTraits<COL_NUM>::vector_type GetRowVector(int row) const {
			auto rowData = GetRowArray(row);
			return typename VectorTypeTraits<COL_NUM>::vector_type(rowData);
		}
		typename VectorTypeTraits<ROW_NUM>::vector_type GetColVector(int col) const {
			auto colData = GetColArray(col);
			return typename VectorTypeTraits<ROW_NUM>::vector_type(colData);
		}

		static Matrix Identity() {
			self_type matrix;
			for (int i = 0; i < Math::Min(COL_NUM, ROW_NUM); i++) {
				matrix[i][i] = 1;
			}
			return matrix;
		}

	private:
		float _values[ROW_NUM][COL_NUM];
	};

	template<int T1, int T2, int T3>
	Matrix<T1, T3> operator*(const Matrix<T1, T2> &lhs, const Matrix<T2, T3> &rhs) {
		Matrix<T1, T3> matrix;
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
	inline Vector4 operator*(const Matrix<4, 4> &lhs, const Vector3 &rhs) {
		Vector4 v4 = Vector4::Of(rhs);
		Vector4 result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result[i] += lhs[i][j] * v4[j];
			}
		}
		return result;
	}
	// the order for template match is operator*(Matrix<4,4>, vector3) > operator*<R, C>, thus we need to specify this implementation.
	inline Vector4 operator*(const Matrix<4, 4> &lhs, const Vector4 &rhs) {
		Vector4 result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result[i] += lhs[i][j] * rhs[j];
			}
		}
		return result;
	}

	typedef Matrix<4, 4> Matrix4x4;
	typedef Matrix<3, 3> Matrix3x3;
	typedef Matrix<3, 4> Matrix3x4;
	typedef Matrix<4, 3> Matrix4x3;
	typedef Matrix4x4 Matrix4;
	typedef Matrix3x3 Matrix3;

	namespace Matrixs {
		static Matrix4x4 CreateMatrix4x4(
				float v00, float v01, float v02, float v03,
				float v10, float v11, float v12, float v13,
				float v20, float v21, float v22, float v23,
				float v30, float v31, float v32, float v33) {
			float values[][4] = {
					{ v00, v01, v02, v03 },
					{ v10, v11, v12, v13 },
					{ v20, v21, v22, v23 },
					{ v30, v31, v32, v33 } };
			return Matrix4x4(values);
		}

		static Matrix3x3 CreateMatrix3x3(
				float v00, float v01, float v02,
				float v10, float v11, float v12,
				float v20, float v21, float v22) {
			float values[][3] = {
					{ v00, v01, v02 },
					{ v10, v11, v12 },
					{ v20, v21, v22 } };
			return Matrix3x3(values);
		}

		static Matrix3x4 CreateMatrix3x4(
				float v00, float v01, float v02, float v03,
				float v10, float v11, float v12, float v13,
				float v20, float v21, float v22, float v23) {
			float values[][4] = {
					{ v00, v01, v02, v03 },
					{ v10, v11, v12, v13 },
					{ v20, v21, v22, v23 } };
			return Matrix<3, 4>(values);
		}

		static Matrix4x3 CreateMatrix4x3(
				float v00, float v01, float v02,
				float v10, float v11, float v12,
				float v20, float v21, float v22,
				float v30, float v31, float v32) {
			float values[][3] = {
					{ v00, v01, v02 },
					{ v10, v11, v12 },
					{ v20, v21, v22 },
					{ v30, v31, v32 } };
			return Matrix4x3(values);
		}
	}
}

#endif