#ifndef _MYGL_CORE_PRIMITIVES_H_
#define _MYGL_CORE_PRIMITIVES_H_

#include <functional>
#include "Vertex.h"
#include "Rasterizer.h"

namespace MyGL {
	// Contains primitive or primitives,
	// since primitives may contains a bunch of same vertices, so that we store the vertices in a single class,
	// and rasterize them at once, thus we do not need additional memory.
	class Primitives {
	public:
		Primitives(vector<Vertex> vertices) : _vertices(vertices) {}
		virtual ~Primitives() {}
		virtual void Rasterize(const std::function<void(const Fragment&)> &callback) = 0;
		virtual bool ValidateVertices() = 0;

		inline int size() const {
			return _vertices.size();
		}
		inline vector<Vertex>& get_vertices() {
			return _vertices;
		}

	protected:
		vector<Vertex> _vertices;
	};

	class Points : public Primitives {
	public:
		Points(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~Points() {}
		override void Rasterize(const std::function<void(const Fragment&)> &callback) {
			for (int i = 0; i < size(); i++) {
				Rasterizer::RasterizePoint(_vertices[i], callback);
			}
		}
		override bool ValidateVertices() {
			return true;
		}
	};
	class Lines : public Primitives {
	public:
		Lines(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~Lines() {}
		override void Rasterize(const std::function<void(const Fragment&)> &callback) {
			for (int i = 0; i < size(); i += 2) {
				Rasterizer::RasterizeLine(_vertices[i], _vertices[i + 1], callback);
			}
		}
		override bool ValidateVertices() {
			return (size() & 1) == 0;
		}
	};
	class LineStrip : public Primitives {
	public:
		LineStrip(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~LineStrip() {}
		override void Rasterize(const std::function<void(const Fragment&)> &callback) {
			for (int i = 1; i < size(); i++) {
				Rasterizer::RasterizeLine(_vertices[i - 1], _vertices[i], callback);
			}
		}
		override bool ValidateVertices() {
			return true;
		}
	};
	class LineLoop : public Primitives {
	public:
		LineLoop(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~LineLoop() {}
		override void Rasterize(const std::function<void(const Fragment&)> &callback) {
			for (int i = 1; i < size(); i++) {
				Rasterizer::RasterizeLine(_vertices[i - 1], _vertices[i], callback);
			}
			if (size() > 1) {
				Rasterizer::RasterizeLine(*_vertices.rbegin(), _vertices[0], callback);
			}
		}
		override bool ValidateVertices() {
			return true;
		}
	};
	class Triangles : public Primitives {
	public:
		Triangles(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~Triangles() {}
		override void Rasterize(const std::function<void(const Fragment&)> &callback) {
			for (int i = 0; i < size(); i += 3) {
				Rasterizer::RasterizeTriangle(_vertices[i], _vertices[i + 1], _vertices[i + 2], callback);
			}
		}
		override bool ValidateVertices() {
			return size() % 3 == 0;
		}
	};
	class TriangleStrip : public Primitives {
	public:
		TriangleStrip(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~TriangleStrip() {}
		override void Rasterize(const std::function<void(const Fragment&)> &callback) {
			for (int i = 0; i < size() - 2; i++) {
				if ((i & 1) == 0) {
					Rasterizer::RasterizeTriangle(_vertices[i], _vertices[i + 1], _vertices[i + 2], callback);
				} else {
					Rasterizer::RasterizeTriangle(_vertices[i + 1], _vertices[i], _vertices[i + 2], callback);
				}
			}
		}
		override bool ValidateVertices() {
			return size() == 0 || size() >= 3;
		}
	};
	class TriangleFan : public Primitives {
	public:
		TriangleFan(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~TriangleFan() {}
		override void Rasterize(const std::function<void(const Fragment&)> &callback) {
			for (int i = 1; i < size() - 1; i++) {
				Rasterizer::RasterizeTriangle(_vertices[0], _vertices[i], _vertices[i + 1], callback);
			}
		}
		override bool ValidateVertices() {
			return size() == 0 || size() >= 3;
		}
	};
	class Quads : public Primitives {
	public:
		Quads(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~Quads() {}
		override void Rasterize(const std::function<void(const Fragment&)> &callback) {
			for (int i = 0; i < size(); i += 4) {
				Rasterizer::RasterizeTriangle(_vertices[i], _vertices[i + 1], _vertices[i + 2], callback);
				Rasterizer::RasterizeTriangle(_vertices[i + 2], _vertices[i + 3], _vertices[i], callback);
			}
		}
		override bool ValidateVertices() {
			return (size() & 3) == 0;
		}
	};
	class QuadStrip : public Primitives {
	public:
		QuadStrip(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~QuadStrip() {}
		override void Rasterize(const std::function<void(const Fragment&)> &callback) {
			for (int i = 0; i < size() - 3; i += 2) {
				auto v1 = _vertices[i], &v2 = _vertices[i + 1], &v3 = _vertices[i + 2], &v4 = _vertices[i + 3];
				if ((i & 1) > 0) {
					std::swap(v1, v2);
				}
				Rasterizer::RasterizeTriangle(v1, v2, v3, callback);
				Rasterizer::RasterizeTriangle(v3, v4, v1, callback);
			}
		}
		override bool ValidateVertices() {
			return size() >= 4 && ((size() - 4) & 1) == 0;
		}
	};
}

#endif