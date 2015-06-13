#include "Clipping.h"

namespace MyGL {
	unique_ptr<Primitives> Clipping::ViewFrustumClipping(unique_ptr<Primitives> primitives, GLState state) {
		//TODO(yifengh): not implemented yet.
		return primitives;
	}
	bool Clipping::BackFaceCulling(const Vertex &v1, const Vertex &v2, const Vertex &v3, GLState state) {
		float determinant = (v2.x() - v1.x()) * (v3.y() - v1.y()) - (v2.y() - v1.y()) * (v3.x() - v1.x());
		CullFaceMask face = GL_FACE_NONE;
		if (determinant > 0) {
			face = state.frontFace == GL_CCW ? GL_BACK : GL_FRONT;
		}
		else if (determinant < 0) {
			face = state.frontFace == GL_CCW ? GL_FRONT : GL_BACK;
		}
		return (state.cullFace & face) > 0;
	}
}