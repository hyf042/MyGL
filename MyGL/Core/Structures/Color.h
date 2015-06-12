#ifndef _MYGL_CORE_STRUCTURES_COLOR_H_
#define _MYGL_CORE_STRUCTURES_COLOR_H_

#include "../Math.h"
#include "../PreDefines.h"
#include "Structures/Vector.h"

namespace MyGL {
	class Color {
	public:
		Color(int _r, int _g, int _b) : _r(_r / 255.0f), _g(_g / 255.0f), _b(_b / 255.0f), _a(1.0f) {}
		Color(int _r, int _g, int _b, int _a) : _r(_r / 255.0f), _g(_g / 255.0f), _b(_b / 255.0f), _a(_a / 255.0f) {}
		Color(float _r, float _g, float _b) : _r(_r), _g(_g), _b(_b), _a(1.0f) {}
		Color(float _r, float _g, float _b, float _a) : _r(_r), _g(_g), _b(_b), _a(_a) {}
		Color(uint32 col) : _a(((col>>24)&0xff)/255.0f), _r(((col>>16)&0xff)/255.0f), _g(((col>>8)&0xff)/255.0f), _b(((col&0xFF)&0xff)/255.0f) {}

		Color Normalize() {
			_a = Math::Clamp(_a, 0.f, 1.f);
			_r = Math::Clamp(_r, 0.f, 1.f);
			_g = Math::Clamp(_g, 0.f, 1.f);
			_b = Math::Clamp(_b, 0.f, 1.f);
			return *this;
		}

		uint32 GetDword() {
			Normalize();
			return ((uint8(_a * 255.0f)) << 24) + ((uint8(_r * 255.0f)) << 16) + ((uint8(_g * 255.0f)) << 8) + ((uint8(_b * 255.0f)) << 0);
		}

		void GetBytes(uint8 &r, uint8 &g, uint8 &b, uint8 &a) {
			a = Math::Clamp(uint32(this->_a * 255.0f), 0u, 255u);
			r = Math::Clamp(uint32(this->_r * 255.0f), 0u, 255u);
			g = Math::Clamp(uint32(this->_g * 255.0f), 0u, 255u);
			b = Math::Clamp(uint32(this->_b * 255.0f), 0u, 255u);
		}

		operator uint32() {
			return GetDword();
		}

		inline float a() const { return _a; }
		inline float r() const { return _r; }
		inline float g() const { return _g; }
		inline float b() const { return _b; }
		inline void set_a(float val) { _a = val; }
		inline void set_r(float val) { _r = val; }
		inline void set_g(float val) { _g = val; }
		inline void set_b(float val) { _b = val; }

		static Color Lerp(const Color &from, const Color &to, float ratio) {
			return Color(
				Math::Lerp(from._r, to._r, ratio),
				Math::Lerp(from._g, to._g, ratio),
				Math::Lerp(from._b, to._b, ratio),
				Math::Lerp(from._a, to._a, ratio));
		}

		friend Color operator*(const Color &color, float scale) {
			return Color(color._r * scale, color._g * scale, color._b * scale, color._a * scale);
		}
		friend Color operator*(const Color &color, const Vector4 &scale) {
			return Color(color._r * scale.x(), color._g * scale.y(), color._b * scale.z(), color._a * scale.w());
		}
		friend Color operator*(const Color &lhs, const Color &rhs) {
			return Color(lhs._r * rhs._r, lhs._g * rhs._g, lhs._b * rhs._b, lhs._a * rhs._a);
		}
		friend Color operator+(const Color &lhs, const Color &rhs) {
			return Color(lhs._r + rhs._r, lhs._g + rhs._g, lhs._b + rhs._b, lhs._a + rhs._a);
		}

	private:
		float _a, _r, _g, _b;
	};

	namespace Colors {
		/* pre defined colors */
		static Color White = Color(1.f, 1.f, 1.f, 1.f);
		static Color Black = Color(0.f, 0.f, 0.f, 1.f);
		static Color Red = Color(1.f, 0.f, 0.f, 1.f);
		static Color Blue = Color(0.f, 0.f, 1.f, 1.f);
		static Color Lime = Color(0.f, 1.f, 0.f, 1.f);
		static Color Marron = Color(0xFF800000);
		static Color Darkred = Color(0xFF8B0000);
		static Color Crimson = Color(0xFFDC143C);
		static Color Darkmagenta = Color(0xFF8B008B);
		static Color Orchid = Color(0xFFDA70D6);
		static Color Thistle = Color(0xFFD8BFD8);
		static Color Plum = Color(0xFFDDA0DD);
		static Color Violet = Color(0xFFEE82EE);
		static Color Slateblue = Color(0xFF6A5ACD);
		static Color Mediumslateblue = Color(0xFF7B68EE);
		static Color Darkblue = Color(0xFF00008B);
		static Color Mediumblue = Color(0xFF0000CD);
		static Color Navy = Color(0xFF000080);
		static Color Royalblue = Color(0xFF4169E1);
		static Color Lightsteelblue = Color(0xFFB0C4DE);
		static Color Aliceblue = Color(0xFFF0F8FF);
		static Color Ghostblue = Color(0xFFF8F8FF);
		static Color Lavender = Color(0xFFE6E6FA);
		static Color Dodgerblue = Color(0xFF1E90FF);
		static Color Steelblue = Color(0xFF00BFFF);
		static Color Deepskyblue = Color(0xFF4169E1);
		static Color Slategray = Color(0xFF808090);
		static Color Lightskyblue = Color(0xFF87CEFA);
		static Color Skyblue = Color(0xFF87CEEB);
		static Color Lightblue = Color(0xFFADD8E6);
		static Color Teal = Color(0xFF008080);
		static Color Darkturquoise = Color(0xFF00CED1);
		static Color Aqua = Color(0x0000FFFF);
		static Color Mediumturquoise = Color(0xFF48D1CC);
		static Color Cadetblue = Color(0xFF5F9EA0);
		static Color Paleturquoise = Color(0xFFAFEEEE);
		static Color Lightcyan = Color(0xFFE0FFFF);
		static Color Azure = Color(0xFFF0FFFF);
		static Color Lightseagreen = Color(0xFF20B2AA);
		static Color Turquoise = Color(0xFFAFEEEE);
		static Color Powderblue = Color(0xFF40E0D0);
		static Color Darkslategray = Color(0xFF2F4F4F);
		static Color Aquamarine = Color(0xFF7FFFD4);
		static Color Springgreen = Color(0xFF00FF7F);
		static Color Mediumseagreen = Color(0xFF3CB371);
		static Color Seagreen = Color(0xFF2E8B57);
		static Color Limegreen = Color(0xFF32CD32);
		static Color Darkgreen = Color(0xFF006400);
		static Color Green = Color(0xFF008000);
		static Color Forestgreen = Color(0xFF228B22);
		static Color Darkseagreen = Color(0xFF8FBC8F);
		static Color Lightgreen = Color(0xFF90EE90);
		static Color Palegreen = Color(0xFF98FB98);
		static Color Mintcream = Color(0xFFF5FFFA);
		static Color Honeydew = Color(0xFFF0FFF0);
		static Color Chartreuse = Color(0xFF7FFF00);
		static Color Olivedrab = Color(0xFF6B8E23);
		static Color Darkolivegreen = Color(0xFF556B2F);
		static Color Yellowgreen = Color(0xFF9ACD32);
		static Color Greenyellow = Color(0xFFADFF2F);
		static Color Beige = Color(0xFFF5F5DC);
		static Color Linen = Color(0xFFFAF0E6);
		static Color Olive = Color(0xFF808000);
		static Color Yellow = Color(0xFFFFFF00);
		static Color Lightyellow = Color(0xFFFFFFE0);
		static Color Ivory = Color(0xFFFFFFF0);
		static Color Darkkhaki = Color(0xFFBDB76B);
		static Color Khaki = Color(0xFFF0E68C);
		static Color Palegold = Color(0xFFEEE8AA);
		static Color Wheat = Color(0xFFF6DEB3);
		static Color Gold = Color(0xFFFFD700);
		static Color Lemonchiffon = Color(0xFFFFFACD);
		static Color Papayawhip = Color(0xFFFFEFD5);
		static Color Darkgold = Color(0xFFB8860B);
		static Color Goldenrod = Color(0xFFDAA520);
		static Color Antiquewhite = Color(0xFFFAEBD7);
		static Color Cornsilk = Color(0xFFFFF8DC);
		static Color Oldlace = Color(0xFFFDF5E6);
		static Color Moccasin = Color(0xFFFFE4B5);
		static Color Navajowhite = Color(0xFFFFDEAD);
		static Color Orange = Color(0xFFFFA500);
		static Color Bisque = Color(0xFFFFE4CE);
		static Color Tan = Color(0xFFD2B48C);
		static Color Darkorange = Color(0xFFFF8C00);
		static Color Burlywood = Color(0xFFDEB887);
		static Color Saddlebrown = Color(0xFF8B4513);
		static Color Sandybrown = Color(0xFFF4A460);
		static Color Blanchedalmond = Color(0xFFFFEBCD);
		static Color Lavenderblush = Color(0xFFFFF0F5);
		static Color Seashell = Color(0xFFFFF5EE);
		static Color Floralwhite = Color(0xFFFFFAF0);
		static Color Snow = Color(0xFFFFFAFA);
		static Color Peru = Color(0xFFCD853F);
		static Color Peachpuff = Color(0xFFFFDAB9);
		static Color Chocolate = Color(0xFFD2691E);
		static Color Sienna = Color(0xFFA0522D);
		static Color Lightsalmon = Color(0xFFFFA07A);
		static Color Coral = Color(0xFFFF7F50);
		static Color Darksalm = Color(0xFFE9967A);
		static Color Mistyrose = Color(0xFFFFE4E1);
		static Color Orangered = Color(0xFFFF4500);
		static Color Salmon = Color(0xFFFA8072);
		static Color Tomato = Color(0xFFFF6347);
		static Color Rosybrow = Color(0xFFBC8F8F);
		static Color Pink = Color(0xFFFFC0CB);
		static Color Indianred = Color(0xFFCD5C5C);
		static Color Lightcoral = Color(0xFFF08080);
		static Color Brown = Color(0xFFA52A2A);
		static Color Firebrik = Color(0xFFB22222);
		static Color Dimgray = Color(0xFF696969);
		static Color Gray = Color(0xFF808080);
		static Color Darkgray = Color(0xFFA9A9A9);
		static Color Silver = Color(0xFFC0C0C0);
		static Color Lightgrey = Color(0xFFD3D3D3);
		static Color Gainsboro = Color(0xFFDCDCDC);
		static Color Whitesmoke = Color(0xFFF5F5F5);
	}
}

#endif