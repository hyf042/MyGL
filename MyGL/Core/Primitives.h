#ifndef _MYGL_CORE_PRIMITIVES_H_
#define _MYGL_CORE_PRIMITIVES_H_

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
		virtual vector<Fragment> Rasterize() = 0;
		virtual bool ValidateVertices() = 0;

		inline int size() const {
			return _vertices.size();
		}

	protected:
		vector<Vertex> _vertices;
	};

	class Points : public Primitives {
	public:
		Points(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~Points() {}
		override vector<Fragment> Rasterize() {
			throw Exception::NotImplementationException();
		}
		override bool ValidateVertices() {
			return true;
		}
	};
	class Lines : public Primitives {
	public:
		Lines(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~Lines() {}
		override vector<Fragment> Rasterize() {
			throw Exception::NotImplementationException();
		}
		override bool ValidateVertices() {
			return (size() & 1) == 0;
		}
	};
	class LineStrip : public Primitives {
	public:
		LineStrip(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~LineStrip() {}
		override vector<Fragment> Rasterize() {
			throw Exception::NotImplementationException();
		}
		override bool ValidateVertices() {
			return true;
		}
	};
	class LineLoop : public Primitives {
	public:
		LineLoop(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~LineLoop() {}
		override vector<Fragment> Rasterize() {
			throw Exception::NotImplementationException();
		}
		override bool ValidateVertices() {
			return true;
		}
	};
	class Triangles : public Primitives {
	public:
		Triangles(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~Triangles() {}
		override vector<Fragment> Rasterize() {
			vector<Fragment> fragments;
			for (int i = 0; i < size(); i+=3) {
				auto _fragments = Rasterizer::RasterizeTriangle(_vertices[i + 0], _vertices[i + 1], _vertices[i + 2]);
				fragments.insert(fragments.end(), _fragments.begin(), _fragments.end());
			}
			return fragments;
		}
		override bool ValidateVertices() {
			return size() % 3 == 0;
		}
	};
	class TriangleStrip : public Primitives {
	public:
		TriangleStrip(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~TriangleStrip() {}
		override vector<Fragment> Rasterize() {
			throw Exception::NotImplementationException();
		}
		override bool ValidateVertices() {
			return size() == 0 || size() >= 3;
		}
	};
	class TriangleFan : public Primitives {
	public:
		TriangleFan(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~TriangleFan() {}
		override vector<Fragment> Rasterize() {
			throw Exception::NotImplementationException();
		}
		override bool ValidateVertices() {
			return size() == 0 || size() >= 3;
		}
	};
	class Quads : public Primitives {
	public:
		Quads(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~Quads() {}
		override vector<Fragment> Rasterize() {
			throw Exception::NotImplementationException();
		}
		override bool ValidateVertices() {
			return size() == 0 || size() >= 4;
		}
	};
	class QuadStrip : public Primitives {
	public:
		QuadStrip(vector<Vertex> vertices) : Primitives(vertices) {}
		virtual ~QuadStrip() {}
		override vector<Fragment> Rasterize() {
			throw Exception::NotImplementationException();
		}
		override bool ValidateVertices() {
			return ((size() - 4) & 1) == 0;
		}
	};
}

#endif