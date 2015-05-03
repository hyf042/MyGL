#include "GL.h"
#include "RenderingContext.h"

namespace MyGL {
	void RenderingContext::swapBuffers() {
		GL::instance().flush();
		const PixelBuffer &buffer = GL::instance().getBuffer(width(), height());
		flushWindow(buffer);
	}
}
