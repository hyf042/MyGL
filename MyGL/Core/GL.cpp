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

	void GL::ClearColor(Color color) {
		_colorBuffer->Clear(color);
	}

	shared_ptr<const PixelBuffer> GL::GetBuffer() {
		return _colorBuffer;
	}
}