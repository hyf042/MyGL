#ifndef _MYGL_CORE_VERTEX_H_
#define _MYGL_CORE_VERTEX_H_

#include "Structures/Vector.h"
#include "Structures/Color.h"

namespace MyGL {
	class Vertex {
	public:
		Vertex() : _color(Colors::White) {}
		Vertex(const Vector3 &position) : _position(position), _color(Colors::White) {}
		Vertex(const Vector3 &position, const Color &color, const Vector2 &uv) : _position(position), _uv(uv), _color(color) {}
		Vertex(const Vector3 &position, const Color &color, const Vector2 &uv, const Vector3 &normal) : _position(position), _uv(uv), _color(color), _normal(normal) {}
		Vertex(const Vector3 &position, const Color &color, const Vector2 &uv, const Vector3 &normal, const Vector3& world_position) : _position(position), _uv(uv), _color(color), _normal(normal), _world_position(world_position) {}

		inline float x() const {
			return _position.x();
		}
		inline float y() const {
			return _position.y();
		}
		inline float z() const {
			return _position.z();
		}
		inline float world_z() const {
			return _world_position.z();
		}
		inline const Vector3& position() const {
			return _position;
		}
		inline const Vector3& world_position() {
			return _world_position;
		}
		inline void set_position(const Vector3 &position) {
			_position = position;
		}
		inline const Vector2& uv() const {
			return _uv;
		}
		inline const Vector3& normal() const {
			return _normal;
		}
		inline const Color& color() const {
			return _color;
		}

		static Vertex Lerp(const Vertex &from, const Vertex &to, float ratio) {
			return Vertex(
				Vector3::Lerp(from._position, to._position, ratio),
				Color::Lerp(from._color, to._color, ratio),
				LerpUVWithPerspectiveCorrectness(from, to, ratio),
				Vector3::Lerp(from._normal, to._normal, ratio),
				Vector3::Lerp(from._world_position, to._world_position, ratio));
		}

		// Lerps the uv of vertex with perspective correctness, see https://en.wikipedia.org/wiki/Texture_mapping#Perspective_correctness for details.
		static Vector2 LerpUVWithPerspectiveCorrectness(const Vertex &from, const Vertex &to, float ratio) {
			return Vector2::Lerp(from._uv / from.world_z(), to._uv / to.world_z(), ratio) / Math::Lerp(1.0f / from.world_z(), 1.0f / to.world_z(), ratio);
		}

	private:
		Vector3 _position;
		Vector2 _uv;
		Vector3 _normal;
		Color _color;
		Vector3 _world_position;
	};

	class Fragment : public Vertex {
	public:
		// the footprint is the size of area where a pixel projection onto the texture space.
		Fragment(const Vertex& vertex, float footprint) : Vertex(vertex) {
			_output_x = static_cast<int>(x());
			_output_y = static_cast<int>(y());
			_footprint = footprint;
		}

		inline int get_output_x() const {
			return _output_x;
		}
		inline int get_output_y() const {
			return _output_y;
		}
		inline float get_footprint() const {
			return _footprint;
		}
	private:
		int _output_x;
		int _output_y;
		//TODO(yifengh): implement rectangle footprint (real footprint from pixel in screen space onto texture space), and further anisotropic filtering.
		float _footprint;
	};
}

#endif