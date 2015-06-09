#include "GL.h"
#include "Common.h"

namespace MyGL {
	void GL::Flush() {
		for (auto &drawCall : _drawCalls) {
			_DoDrawCall(drawCall);
		}
		_drawCalls.clear();
	}

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

	weak_ptr<const PixelBuffer> GL::GetBuffer() {
		return _colorBuffer;
	}

	void GL::_DoDrawCall(const unique_ptr<DrawCall> &drawCall) {
		for (auto &primitive : drawCall->PullPrimitives()) {
			auto fragments = primitive->Rasterize();
			for (auto &fragment : fragments) {
				int x = static_cast<int>(fragment.x());
				int y = static_cast<int>(fragment.y());
				if (Math::InRange(x, 0, width() - 1) && Math::InRange(y, 0, height() - 1)) {
					_colorBuffer->SetColor(x, y, fragment.color);
				}
			}
		}
	}
}