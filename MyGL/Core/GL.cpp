#include "GL.h"
#include "Common.h"

namespace MyGL {
	void GL::Flush() {}

	PixelBuffer GL::GetBuffer(int width, int height) {
		PixelBuffer buffer(width, height);
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				buffer.SetColor(i, j, Colors::Red);
			}
		}
		return buffer;
	}
}