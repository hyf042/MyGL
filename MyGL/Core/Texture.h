#ifndef _MYGL_CORE_TEXTURE_H_
#define _MYGL_CORE_TEXTURE_H_

#include "PreDefines.h"
#include "Structures/Buffer.h"
#include "Structures/Color.h"
#include "Consts.h"

namespace MyGL {
	class Texture {
	public:
		Texture() : _borderColor(Colors::White) { _Init(); }
		Texture(int width, int height) : _buffer(new ColorBuffer(width, height)), _borderColor(Colors::White) {
			_Init();
		}
		Texture(int width, int height, const uint8 *bytes) : _borderColor(Colors::White) {
			_Init();
			SetTexture(width, height, bytes);
		}
		void SetTexture(int width, int height, const uint8 *bytes) {
			_buffer = make_unique<ColorBuffer>(width, height, bytes);
		}
		Color GetColor(int x, int y) {
			return Color(_buffer->Get(x, y));
		}
		Color GetColor(const Vector2 &uv) {
			if (_horizWrapMode == GL_CLAMP_TO_BORDER || _vertWrapMode == GL_CLAMP_TO_BORDER) {
				if (!Math::InRange(uv.x(), 0.0f, 1.0f) || !Math::InRange(uv.y(), 0.0f, 1.0f)) {
					return _borderColor;
				}
			}
			float x = _Wrap(_horizWrapMode, uv.x());
			float y = _Wrap(_vertWrapMode, uv.y());
			int tx = static_cast<int>(Math::Round(x * width() - 0.5f));
			int ty = static_cast<int>(Math::Round(y * height() - 0.5f));
			tx = Math::Clamp(tx, 0, width() - 1);
			ty = Math::Clamp(ty, 0, height() - 1);
			return GetColor(tx, ty);
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
		void SetWrapMode(WrapMode horizMode, WrapMode vertMode) {
			_horizWrapMode = horizMode;
			_vertWrapMode = vertMode;
		}
	private:
		void _Init() {
			_horizWrapMode = GL_REPEAT;
			_vertWrapMode = GL_REPEAT;
			_borderColor = Colors::Black;
		}
		float _Wrap(WrapMode mode, float v) {
			if (Math::InRange(v, 0.0f, 1.0f)) {
				return v;
			}
			switch (mode) {
				case GL_REPEAT: {
					return Math::Mod(v, 1.0f);
				}
				case GL_MIRRORED_REPEAT: {
					int div = static_cast<int>(v >= 0 ? floor(v) : ceil(v));
					if (div & 1) {
						return 1.0f - Math::Mod(v, 1.0f);
					}
					else {
						return Math::Mod(v, 1.0f);
					}
				}
				case GL_CLAMP_TO_EDGE: {
					return v < 0 ? 0.0f : 1.0f;
				}
				default:
					throw Exception::NotImplementationException();
			}
		}

		unique_ptr<ColorBuffer> _buffer;
		WrapMode _horizWrapMode;
		WrapMode _vertWrapMode;
		Color _borderColor;
	};
}

#endif