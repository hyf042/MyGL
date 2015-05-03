#include "PixelBuffer.h"
#include "PixelFormat.h"

namespace MyGL {
	PixelBuffer::PixelBuffer(int width, int height) : PixelBuffer(width, height, PixelFormat()) {}

	PixelBuffer::PixelBuffer(int width, int height, PixelFormat pixelFormat) {
		m_width = width;
		m_height = height;
		m_pixels = vector<uint8>(width * height * 4);
	}

	PixelBuffer PixelBuffer::clone() const {
		return PixelBuffer(*this);
	}
}