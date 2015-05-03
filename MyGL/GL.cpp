#include "Common.h"
#include "GL.h"

namespace MyGL {
	void GL::flush() {}

	PixelBuffer GL::getBuffer(int width, int height) {
		PixelBuffer buffer(width, height);
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				buffer.setColor(i, j, Colors::Red);
			}
		}
		return buffer;
	}
}