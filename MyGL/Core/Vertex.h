#ifndef _MYGL_CORE_VERTEX_H_
#define _MYGL_CORE_VERTEX_H_

#include "Structures/Vector.h"
#include "Structures/Color.h"

namespace MyGL {
	class Vertex {
	public:
		Vector3 position;
		Vector2 uv;
		Vector3 normal;
		Color color;

		Vertex() : color(Colors::White) {}
		Vertex(const Vector3 &position) : position(position), color(Colors::White) {}
		Vertex(const Vector3 &position, const Color &color, const Vector2 &uv) : position(position), uv(uv), color(color) {}
		Vertex(const Vector3 &position, const Color &color, const Vector2 &uv, const Vector3 &normal) : position(position), uv(uv), color(color), normal(normal) {}

		inline float x() const {
			return position.x();
		}
		inline float y() const {
			return position.y();
		}
		inline float z() const {
			return position.z();
		}

		static Vertex Lerp(const Vertex &from, const Vertex &to, float ratio) {
			return Vertex(
				Vector3::Lerp(from.position, to.position, ratio),
				Color::Lerp(from.color, to.color, ratio),
				Vector2::Lerp(from.uv, to.uv, ratio),
				Vector3::Lerp(from.normal, to.normal, ratio));
		}
	};

	typedef Vertex Fragment;
}

#endif