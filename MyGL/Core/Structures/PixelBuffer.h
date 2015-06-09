#ifndef _MYGL_CORE_STRUCTURES_PIXELBUFFER_H_
#define _MYGL_CORE_STRUCTURES_PIXELBUFFER_H_

#include "Color.h"
#include "../PreDefines.h"
#include "../Interfaces/IClonable.h"

namespace MyGL {
	class PixelBuffer : public IClonable<PixelBuffer> {
	public:
		PixelBuffer(int width, int height);
		PixelBuffer(int width, int height, PixelFormat pixelFormat);
		virtual ~PixelBuffer() {}

		int width() const { return _width; }
		int height() const { return _height; }

		int GetIndex(int x, int y) {
			return y * width() + x;
		}

		void Clear(Color color) {
			memset(&_pixels[0], color.GetDword(), sizeof(uint32) * width() * height());
		}

		void SetData(int index, int elementIndex, uint8 byte) {
			_pixels[(index << 2) + elementIndex] = byte;
		}
		void SetData(int x, int y, int elementIndex, uint8 byte) {
			SetData(GetIndex(x, y), elementIndex, byte);
		}
		void SetColor(int index, Color color) {
			uint8 r, g, b, a;
			color.GetBytes(r, g, b, a);
			_pixels[(index << 2) + 0] = r;
			_pixels[(index << 2) + 1] = g;
			_pixels[(index << 2) + 2] = b;
			_pixels[(index << 2) + 3] = a;
		}
		void SetColor(int x, int y, Color color) {
			SetColor(GetIndex(x, y), color);
		}

		const uint8* GetPixelRaw() const {
			if (_pixels.size() > 0) {
				return &_pixels[0];
			}
			else {
				return nullptr;
			}
		}

		override PixelBuffer Clone() const {
			return PixelBuffer(*this);
		}
		override shared_ptr<PixelBuffer> ClonePtr() const {
			return make_shared<PixelBuffer>(*this);
		}

	private:
		vector<uint8> _pixels;
		int _width;
		int _height;
	};
}

#endif