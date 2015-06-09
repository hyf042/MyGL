#ifndef _MYGL_CORE_PRIMITIVESBUILDER_H_
#define _MYGL_CORE_PRIMITIVESBUILDER_H_

#include "Primitives.h"

namespace MyGL {
	class PrimitivesBuilder {
	public:
		static unique_ptr<PrimitivesBuilder> Begin(PrimitivesType mode) {
			return unique_ptr<PrimitivesBuilder>(new PrimitivesBuilder(mode));
		}

		void AddVertex(const Vertex &vertex) {
			_vertices.push_back(vertex);
		}

		unique_ptr<Primitives> Build() {
			switch (_mode) {
			case GL_POINTS:
				return make_unique<Points>(_vertices);
			case GL_LINES:
				return make_unique<Lines>(_vertices);
			case GL_LINE_STRIP:
				return make_unique<LineStrip>(_vertices);
			case GL_LINE_LOOP:
				return make_unique<LineLoop>(_vertices);
			case GL_TRIANGLES:
				return make_unique<Triangles>(_vertices);
			case GL_TRIANGLE_STRIP:
				return make_unique<TriangleStrip>(_vertices);
			case GL_TRIANGLE_FAN:
				return make_unique<TriangleFan>(_vertices);
			case GL_QUADS:
				return make_unique<Quads>(_vertices);
			case GL_QUAD_STRIP:
				return make_unique<QuadStrip>(_vertices);
			default:
				throw Exception::NotImplementationException();
			}
		}
	private:
		PrimitivesBuilder(PrimitivesType mode) : _mode(mode), _vertices() {}

		PrimitivesType _mode;
		vector<Vertex> _vertices;
	};
}

#endif