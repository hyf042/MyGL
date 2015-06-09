#ifndef _MYGL_CORE_CLIPPING_H_
#define _MYGL_CORE_CLIPPING_H_

#include "GLState.h"
#include "Primitive.h"

namespace MyGL {
	class Clipping {
	public:
		virtual vector<Primitive> ViewFrustumClipping(const vector<Primitive> primitives, GLState state) = 0;
		virtual vector<Primitive> BackFaceCulling(const vector<Primitive> primitives, GLState state) = 0;
	};
}

#endif