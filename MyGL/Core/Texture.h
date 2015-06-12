#ifndef _MYGL_CORE_TEXTURE_H_
#define _MYGL_CORE_TEXTURE_H_

#include "PreDefines.h"
#include "Structures/Buffer.h"
#include "Structures/Color.h"

namespace MyGL {
	class Texture {
	public:
		Texture() {}
		Texture(int width, int height) : _buffer(new ColorBuffer(width, height)) {}
		Texture(int width, int height, const uint8 *bytes) {
			SetTexture(width, height, bytes);
		}
		void SetTexture(int width, int height, const uint8 *bytes) {
			_buffer = make_unique<ColorBuffer>(width, height, bytes);
		}
		Color GetColor(int x, int y) {
			return Color(_buffer->Get(x, y));
		}
		const uint8* GetRawBytes() {
			return _buffer->GetRawBytes();
		}
		inline int width() {
			return _buffer->width();
		}
		inline int height() {
			return _buffer->height();
		}
	private:
		unique_ptr<ColorBuffer> _buffer;
	};
}

#endif