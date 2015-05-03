#ifndef _MYGL_GL_H_
#define _MYGL_GL_H_

#include "Common.h"

namespace MyGL {
	class GL {
	private:
		GL() {}
	public:
		static GL& instance() {
			static GL _instance;
			return _instance;
		}

		void flush();
		PixelBuffer getBuffer(int width, int height);
	};
}

#endif