#include "Shader.h"
#include "GL.h"

namespace MyGL {
	Vertex Shader::VertexFunc(const Vertex& vertex) {
		return vertex;
	}
	Color Shader::FragmentFunc(weak_ptr<Texture> texture, const Fragment &fragment) {
		if (texture.lock()) {
			return fragment.color * GL::GetTexture2D(texture, fragment.uv);
		}
		else {
			return fragment.color;
		}
	}
}