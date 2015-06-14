#include "Shader.h"
#include "GL.h"

namespace MyGL {
	Vertex Shader::VertexFunc(const Vertex& vertex) {
		return vertex;
	}
	Color Shader::FragmentFunc(weak_ptr<Texture> texture, const Fragment &fragment) {
		return GL::Instance().GetColor(fragment) * GL::GetTexture2D(texture, fragment);;
	}
}