#ifndef _MYGL_CORE_GL_H_
#define _MYGL_CORE_GL_H_

#include "Common.h"

namespace MyGL {
	class GL {
	public:
		static GL& Instance() {
			static GL _instance;
			return _instance;
		}

		void Flush();
		PixelBuffer GetBuffer(int width, int height);

	private:
		GL() {}
	};
}

#endif