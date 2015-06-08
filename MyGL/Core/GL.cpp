#include "GL.h"
#include "Common.h"

namespace MyGL {
	void GL::Flush() {}

	void GL::Clear(bool colorBuffer, bool depthBuffer, bool stencilBuffer) {
		if (colorBuffer) {
			ClearColor(Colors::Black);
		}
		if (depthBuffer) {
			ClearDepth();
		}
		if (stencilBuffer) {
			ClearStencil();
		}
	}

	GL& GL::Translate(const Vector3& offset) {
		_state.modelMatrix = Transforms::CreateTranslate(offset.x(), offset.y(), offset.z()) * _state.modelMatrix;
		return *this;
	}

	GL& GL::Scale(const Vector3& scale) {
		_state.modelMatrix = Transforms::CreateScale(scale.x(), scale.y(), scale.z()) * _state.modelMatrix;
		return *this;
	}

	GL& GL::Rotate(float angle, Vector3 direction) {
		_state.modelMatrix = Transforms::CreateRotation(angle, direction.x(), direction.y(), direction.z()) * _state.modelMatrix;
		return *this;
	}


	void GL::ClearColor(Color color) {
		_colorBuffer->Clear(color);
	}

	shared_ptr<const PixelBuffer> GL::GetBuffer() {
		return _colorBuffer;
	}
}