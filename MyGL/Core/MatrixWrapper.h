#ifndef _MYGL_CORE_MATRIXWRAPPER_H_
#define _MYGL_CORE_MATRIXWRAPPER_H_

#include <stack>
#include "Structures/Matrix.h"
#include "Transforms.h"

namespace MyGL {
	using std::stack;

	class MatrixWrapper {
	public:
		MatrixWrapper() {
			_matrix = Matrix4::Identity();
		}

		MatrixWrapper& LoadIdentity() {
			_matrix = Matrix4::Identity();
			_matrixStack = stack<Matrix4>();
			return *this;
		}

		MatrixWrapper& Translate(float x, float y, float z) {
			return Translate(Vector3(x, y, z));
		}
		MatrixWrapper& Translate(const Vector3 &offset) {
			_matrix = _matrix * Transforms::MakeTranslate(offset.x(), offset.y(), offset.z());
			return *this;
		}
		MatrixWrapper& Scale(float x, float y, float z) {
			return Scale(Vector3(x, y, z));
		}
		MatrixWrapper& Scale(float scale) {
			return Scale(Vector3(scale, scale, scale));
		}
		MatrixWrapper& Scale(const Vector3 &scale) {
			_matrix = _matrix * Transforms::MakeScale(scale.x(), scale.y(), scale.z());
			return *this;
		}

		MatrixWrapper& Rotate(float angle, float x, float y, float z) {
			return Rotate(angle, Vector3(x, y, z));
		}
		MatrixWrapper& Rotate(float angle, Vector3 direction) {
			_matrix = _matrix * Transforms::MakeRotation(angle, direction.x(), direction.y(), direction.z());
			return *this;
		}

		MatrixWrapper& Push() {
			_matrixStack.push(_matrix);
			return *this;
		}
		MatrixWrapper& Pop() {
			if (_matrixStack.empty()) {
				throw Exception::ArrayEmptyException();
			}
			_matrix = _matrixStack.top();
			_matrixStack.top();
			return *this;
		}

		MatrixWrapper& Multiply(const Matrix4 &left) {
			_matrix = _matrix * left;
			return *this;
		}

		MatrixWrapper& operator=(const Matrix4& other) {
			_matrix = other;
			return *this;
		}

		void set_matrix(const Matrix4 &matrix) {
			_matrix = matrix;
		}
		const Matrix4& get_matrix() const {
			return _matrix;
		}
	private:
		stack<Matrix4> _matrixStack;
		Matrix4 _matrix;
	};
}

#endif