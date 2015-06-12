#include "RenderingContext.h"
#include "../Core/GL.h"

namespace MyGL {
	void RenderingContext::SwapBuffers() {
		GL::Instance().Flush();
		FlushWindow(GL::Instance().GetBuffer());
	}
}
