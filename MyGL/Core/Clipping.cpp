#include "Clipping.h"

namespace MyGL {
	vector<Primitives> Clipping::ViewFrustumClipping(const vector<Primitives> primitives, GLState state) {
		throw Exception::NotImplementationException();
	}
	bool Clipping::BackFaceCulling(const Vertex &v1, const Vertex &v2, const Vertex &v3, GLState state) {
		float determinant = (v2.position.x() - v1.position.x()) * (v3.position.y() - v1.position.y()) - (v2.position.y() - v1.position.y()) * (v3.position.x() - v1.position.x());
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