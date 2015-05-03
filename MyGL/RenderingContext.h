#ifndef _MYGL_RENDERING_CONTEXT_
#define _MYGL_REDNERING_CONTEXT_

#include "PreDefines.h"
#include "PixelBuffer.h"

namespace MyGL {
	class RenderingContext {
	public:
		virtual int width() const = 0;
		virtual int height() const = 0;

		virtual void createWindow(int width, int height, const string &title) = 0;
		
		virtual void swapBuffers();
	protected:
		virtual void flushWindow(const PixelBuffer& buffer) = 0;
	};
}

#endif