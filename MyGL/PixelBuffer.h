#ifndef _MYGL_PIXEL_BUFFER_H_
#define _MYGL_PIXEL_BUFFER_H_

#include "Color.h"
#include "PreDefines.h"

namespace MyGL {
	class PixelBuffer {
		vector<uint8> m_pixels;
		int m_width;
		int m_height;
	public:
		PixelBuffer(int width, int height);
		PixelBuffer(int width, int height, PixelFormat pixelFormat);

		int width() const { return m_width; }
		int height() const { return m_height; }
		int getIndex(int x, int y) {
			return y * width() + x;
		}

		void setData(int index, int elementIndex, uint8 byte) {
			m_pixels[(index << 2) + elementIndex] = byte;
		}
		void setData(int x, int y, int elementIndex, uint8 byte) {
			setData(getIndex(x, y), elementIndex, byte);
		}
		void setColor(int index, Color color) {
			uint8 r, g, b, a;
			color.getBytes(r, g, b, a);
			m_pixels[(index << 2) + 0] = r;
			m_pixels[(index << 2) + 1] = g;
			m_pixels[(index << 2) + 2] = b;
			m_pixels[(index << 2) + 3] = a;
		}
		void setColor(int x, int y, Color color) {
			setColor(getIndex(x, y), color);
		}

		const uint8* getPixelRaw() const {
			if (m_pixels.size() > 0) {
				return &m_pixels[0];
			}
			else {
				return NULL;
			}
		}

		PixelBuffer clone() const;
	};
}

#endif