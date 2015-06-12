#ifndef _MYGL_CONTEXT_RENDERINGCONTEXT_H_
#define _MYGL_CONTEXT_RENDERINGCONTEXT_H_

#include "../Core/PreDefines.h"
#include "../Core/Structures/Buffer.h"

namespace MyGL {
	class RenderingContext {
	public:
		virtual ~RenderingContext() {}

		virtual int width() const = 0;
		virtual int height() const = 0;

		virtual void CreateWindow(int width, int height, const string &title) = 0;
		
		virtual void SwapBuffers();
	protected:
		virtual void FlushWindow(weak_ptr<const ColorBuffer> buffer) = 0;
	};
}

#endif