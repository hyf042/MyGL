#ifndef _MYGL_CORE_TEXTURE_H_
#define _MYGL_CORE_TEXTURE_H_

#include "PreDefines.h"
#include "Structures/Buffer.h"
#include "Structures/Color.h"
#include "Consts.h"
#include "Math.h"
#include "Vertex.h"

namespace MyGL {
	class Texture {
	public:
		Texture() : _borderColor(Colors::White) {}
		Texture(int width, int height) : _buffer(new ColorBuffer(width, height)), _borderColor(Colors::White) {}
		Texture(int width, int height, const uint8 *bytes) : _borderColor(Colors::White) {
			SetTexture(width, height, bytes);
		}
		void SetTexture(int width, int height, const uint8 *bytes) {
			_buffer = make_shared<ColorBuffer>(width, height, bytes);
			LoadMipmap();
		}
		Color GetColor(int x, int y) {
			return Color(_buffer->Get(x, y));
		}
		Color GetColor(const Fragment &fragment) {
			const Vector2 &uv = fragment.uv();

			// Deal with wrapMode.
			if (_horizWrapMode == GL_CLAMP_TO_BORDER || _vertWrapMode == GL_CLAMP_TO_BORDER) {
				if (!Math::InRange(uv.x(), 0.0f, 1.0f) || !Math::InRange(uv.y(), 0.0f, 1.0f)) {
					return _borderColor;
				}
			}
			Vector2 new_uv = Vector2(_Wrap(_horizWrapMode, uv.x()), _Wrap(_vertWrapMode, uv.y()));

			// Deal with texture filtering.
			float footprint = fragment.get_footprint() * width();
			float level = Math::Log2(footprint);
			
			if (footprint <= 1.0f) {
				return _GetColorByFilter(_buffer, new_uv, _maxFilter);
			}
			else {
				// nearest
				switch (_levelFilter) {
				case GL_LINEAR: {
					int lower_level = Math::Clamp(static_cast<int>(Math::Floor(level)), _mipmapMinLOD, _mipmapMaxLOD);
					int upper_level = Math::Clamp(static_cast<int>(Math::Ceil(level)), _mipmapMinLOD, _mipmapMaxLOD);
					if (lower_level >= upper_level) {
						return _GetColorByFilter(_mipmap[lower_level], new_uv, _minFilter);
					}
					else {
						return Color::Lerp(
							_GetColorByFilter(_mipmap[lower_level], new_uv, _minFilter),
							_GetColorByFilter(_mipmap[upper_level], new_uv, _minFilter),
							(level - lower_level) / (upper_level - lower_level));
					}
				}
				case GL_NEAREST: {
					int current_level = Math::Clamp(static_cast<int>(Math::Round(level)), _mipmapMinLOD, _mipmapMaxLOD);
					return _GetColorByFilter(_mipmap[current_level], new_uv, _minFilter);
				}
				default:
					throw Exception::NotImplementationException();
				}
			}
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
		void SetWrapMode(TextureWrapMode horizMode, TextureWrapMode vertMode) {
			_horizWrapMode = horizMode;
			_vertWrapMode = vertMode;
		}
		void SetHorizontalWrapMode(TextureWrapMode mode) {
			_horizWrapMode = mode;
		}
		void SetVerticalWrapMode(TextureWrapMode mode) {
			_vertWrapMode = mode;
		}
		void SetMinFilter(TextureFilterMode filter) {
			_minFilter = filter;
		}
		void SetMaxFilter(TextureFilterMode filter) {
			_maxFilter = filter;
		}
		void SetLevelFilter(TextureFilterMode filter) {
			_levelFilter = filter;
		}
		void SetMinMipmapLOD(int minLOD) {
			_mipmapMinLOD = Math::Clamp(minLOD, 0, _mipmapLevel);
		}
		void SetMaxMinmapLOD(int maxLOD) {
			_mipmapMaxLOD = Math::Clamp(maxLOD, 0, _mipmapLevel);
		}
		void LoadMipmap(int mipmapLevel = -1) {
			assert(width() == height() && Math::IsPowerOf2(width()));

			int maxLog = static_cast<int>(Math::Log2(static_cast<float>(width())));
			shared_ptr<ColorBuffer> current_texture = _buffer;

			if (mipmapLevel < 0) {
				_mipmapLevel = maxLog;
			}
			else {
				_mipmapLevel = Math::Min(mipmapLevel, maxLog);
			}
			_mipmapMinLOD = 0;
			_mipmapMaxLOD = _mipmapLevel;
			_mipmap.clear();
			_mipmap.push_back(current_texture);
			for (int i = 1; i <= _mipmapLevel; i++) {
				current_texture = _ShrinkBy2(current_texture);
				_mipmap.push_back(current_texture);
			}
		}

	protected:
		float _Wrap(TextureWrapMode mode, float v) {
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

		static Color _GetColorByFilter(shared_ptr<ColorBuffer> &buffer, const Vector2 &uv, TextureFilterMode filter) {
			float ux = uv.x() * buffer->width() - 0.5f;
			float uy = uv.y() * buffer->height() - 0.5f;
			switch (filter) {
				case GL_NEAREST: {
					int tx = static_cast<int>(Math::Round(ux));
					int ty = static_cast<int>(Math::Round(uy));
					buffer->Clamp(tx, ty);
					return buffer->Get(tx, ty);
				}
				case GL_LINEAR: {
					int left = static_cast<int>(Math::Floor(ux));
					int bottom = static_cast<int>(Math::Floor(uy));
					int right = static_cast<int>(Math::Ceil(ux));
					int top = static_cast<int>(Math::Ceil(uy));
					buffer->Clamp(left, bottom);
					buffer->Clamp(right, top);
					float horiz_lerp_ratio = left == right ? 0 : (ux - left) / (right - left);
					float vert_lerp_ratio = bottom == top ? 0 : (uy - bottom) / (top - bottom);
					Color bottom_lerp = Color::Lerp(buffer->Get(left, bottom), buffer->Get(right, bottom), horiz_lerp_ratio);
					Color top_lerp = Color::Lerp(buffer->Get(left, top), buffer->Get(right, top), horiz_lerp_ratio);
					return Color::Lerp(bottom_lerp, top_lerp, vert_lerp_ratio);
				}
				default:
					throw Exception::NotImplementationException();
			}
		}

		static shared_ptr<ColorBuffer> _ShrinkBy2(shared_ptr<ColorBuffer> &buffer) {
			assert(buffer->width() == buffer->height() && Math::IsPowerOf2(buffer->width()));

			int size = buffer->width();
			int half_size = size >> 1;
			if (half_size == 0) {
				return nullptr;
			}
			auto new_buffer = make_shared<ColorBuffer>(half_size, half_size);
			for (int i = 0; i < size; i += 2) {
				for (int j = 0; j < size; j += 2) {
					auto color = (Color(buffer->Get(i, j)) 
						+ Color(buffer->Get(i, j + 1))
						+ Color(buffer->Get(i + 1, j))
						+ Color(buffer->Get(i + 1, j + 1))) * .25f;
					new_buffer->SetData(i >> 1, j >> 1, color.GetDword());
				}
			}
			return new_buffer;
		}

	private:
		int _mipmapLevel = 0;
		int _mipmapMinLOD = 0;
		int _mipmapMaxLOD = 0;
		shared_ptr<ColorBuffer> _buffer;
		vector<shared_ptr<ColorBuffer>> _mipmap;
		TextureWrapMode _horizWrapMode = GL_REPEAT;
		TextureWrapMode _vertWrapMode = GL_REPEAT;
		TextureFilterMode _minFilter = GL_NEAREST;
		TextureFilterMode _maxFilter = GL_NEAREST;
		TextureFilterMode _levelFilter = GL_NEAREST;
		Color _borderColor = Colors::White;
	};
}

#endif