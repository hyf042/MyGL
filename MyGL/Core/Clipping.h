#ifndef _MYGL_CORE_CLIPPING_H_
#define _MYGL_CORE_CLIPPING_H_

#include "GLState.h"
#include "Primitives.h"

namespace MyGL {
	class Clipping {
	public:
		virtual vector<Primitives> ViewFrustumClipping(const vector<Primitives> primitives, GLState state);
		virtual bool BackFaceCulling(const Vertex &v1, const Vertex &v2, const Vertex &v3, GLState state);
	};
}

#endif