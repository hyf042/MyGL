#ifndef _MYGL_CORE_GL_H_
#define _MYGL_CORE_GL_H_

#include "Common.h"
#include "MatrixWrapper.h"

namespace MyGL {
	class GL {
	public:
		static GL& Instance() {
			static GL _instance;
			return _instance;
		}

		void Init(int width, int height) {
			_colorBuffer = make_shared<PixelBuffer>(width, height);
			Viewport(0, 0, static_cast<float>(width), static_cast<float>(height));
		}

		void Clear(bool colorBuffer = true, bool depthBuffer = false, bool stencilBuffer = false);
		void ClearColor(Color color);
		void ClearDepth() { throw Exception::NotImplementationException(); }
		void ClearStencil() { throw Exception::NotImplementationException(); }

		void MatrixMode(MatrixMode matrixMode) {
			switch (matrixMode) {
			case GL_MODEVIEW:
				break;
			case GL_PROJECTION:
				break;
			default:
				throw Exception::NotImplementationException();
			}
		}
		void LoadIdentity() {
			_GetCurrentMatrixWrapper().LoadIdentity();
		}

		void Viewport(Rect rect, float minZ = 0.0f, float maxZ = 1.0f) {
			_state.viewportMatrix = Transforms::MakeViewport(rect, minZ, maxZ);
		}
		void Viewport(float minX, float minY, float width, float height, float minZ = 0.0f, float maxZ = 1.0f) {
			_state.viewportMatrix = Transforms::MakeViewport(minX, minY, width, height, minZ, maxZ);
		}
		void LookAt(Vector3 eye, Vector3 at, Vector3 up) {
			_state.modelViewMatrix.Multiply(Transforms::MakeLookAt(eye, at, up));
		}
		void Perspective(float fovy, float aspect, float zNear, float zFar) {
			_state.projectionMatrix = Transforms::MakePerspectiveFov(fovy, aspect, zNear, zFar);
		}
		void Ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
			_state.projectionMatrix = Transforms::MakeOrtho(left, right, bottom, top, zNear, zFar);
		}

		GL& Translate(float x, float y, float z) {
			_GetCurrentMatrixWrapper().Translate(x, y, z);
			return *this;
		}
		GL& Translate(const Vector3 &offset) {
			_GetCurrentMatrixWrapper().Translate(offset);
			return *this;
		}
		GL& Scale(float x, float y, float z) {
			_GetCurrentMatrixWrapper().Scale(x, y, z);
			return *this;
		}
		GL& Scale(float scale) {
			_GetCurrentMatrixWrapper().Scale(scale);
			return *this;
		}
		GL& Scale(const Vector3 &scale) {
			_GetCurrentMatrixWrapper().Scale(scale);
			return *this;
		}
		GL& Rotate(float angle, float x, float y, float z) {
			_GetCurrentMatrixWrapper().Rotate(angle, x, y, z);
			return *this;
		}
		GL& Rotate(float angle, Vector3 direction) {
			_GetCurrentMatrixWrapper().Rotate(angle, direction);
			return *this;
		}

		const Matrix4& GetCurrentMatrix() {
			return _state.GetCurrentMatrix().get_matrix();
		}

		// Test
		Vector3 TestTransform(Vector3 point) {
			return _state.ModelToViewport(point);
		}

		void Flush();
		shared_ptr<const PixelBuffer> GetBuffer();

	private:
		GL() {}
		MatrixWrapper& _GetCurrentMatrixWrapper() {
			return _state.GetCurrentMatrix();
		}

		GLState _state;
		shared_ptr<Clipping> _clipping;
		shared_ptr<PixelBuffer> _colorBuffer;
		shared_ptr<PixelBuffer> _depthBuffer;
		shared_ptr<PixelBuffer> _stencilBuffer;
	};
}

#endif