#ifndef _MYGL_CORE_GL_H_
#define _MYGL_CORE_GL_H_

#include "Common.h"

namespace MyGL {
	class GLState {
	public:
		Matrix4 modelMatrix;
		Matrix4 viewMatrix;
		Matrix4 projectionMatrix;

		GLState() {
			viewMatrix = Transforms::CreateLookAt(Vector3::Zero(), Vectors::Forward, Vectors::Up);
			modelMatrix = Matrix4::Identity();
			projectionMatrix = Matrix4::Identity();
		}
	};

	class GL {
	public:
		static GL& Instance() {
			static GL _instance;
			return _instance;
		}

		void Init(int width, int height) {
			_colorBuffer = make_shared<PixelBuffer>(width, height);
		}

		void Clear(bool colorBuffer = true, bool depthBuffer = false, bool stencilBuffer = false);
		void ClearColor(Color color);
		void ClearDepth() { throw Exception::NotImplementationException(); }
		void ClearStencil() { throw Exception::NotImplementationException(); }

		void LookAt(Vector3 eye, Vector3 at, Vector3 up) {
			_state.viewMatrix = Transforms::CreateLookAt(eye, at, up);
		}

		GL& Translate(float x, float y, float z) {
			return Translate(Vector3(x, y, z));
		}
		GL& Translate(const Vector3 &offset);
		GL& Scale(float x, float y, float z) {
			return Scale(Vector3(x, y, z));
		}
		GL& Scale(float scale) {
			return Scale(Vector3(scale, scale, scale));
		}
		GL& Scale(const Vector3 &scale);
		GL& Rotate(float angle, float x, float y, float z) {
			return Rotate(angle, Vector3(x, y, z));
		}
		GL& Rotate(float angle, Vector3 direction);

		// Test
		Vector3 TestTransform(Vector3 point) {
			return _state.viewMatrix * _state.modelMatrix * point;
		}

		void Flush();
		shared_ptr<const PixelBuffer> GetBuffer();

	private:
		GL() {}

		GLState _state;
		shared_ptr<PixelBuffer> _colorBuffer;
		shared_ptr<PixelBuffer> _depthBuffer;
		shared_ptr<PixelBuffer> _stencilBuffer;
	};
}

#endif