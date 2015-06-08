#include "RenderingContext.h"
#include "../Core/GL.h"

namespace MyGL {
	void RenderingContext::SwapBuffers() {
		GL::Instance().Flush();
		auto buffer = GL::Instance().GetBuffer();
		FlushWindow(buffer);
	}
}
