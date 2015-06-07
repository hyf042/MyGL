#include "RenderingContext.h"
#include "../Core/GL.h"

namespace MyGL {
	void RenderingContext::SwapBuffers() {
		GL::Instance().Flush();
		const PixelBuffer &buffer = GL::Instance().GetBuffer(width(), height());
		FlushWindow(buffer);
	}
}
