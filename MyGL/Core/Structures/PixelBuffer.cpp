#include "PixelBuffer.h"
#include "PixelFormat.h"

namespace MyGL {
	PixelBuffer::PixelBuffer(int width, int height) : PixelBuffer(width, height, PixelFormat()) {}

	PixelBuffer::PixelBuffer(int width, int height, PixelFormat pixelFormat) {
		_width = width;
		_height = height;
		_pixels = vector<uint8>(width * height * 4);
	}

	PixelBuffer PixelBuffer::Clone() const {
		return PixelBuffer(*this);
	}
}