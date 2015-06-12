#ifndef _MYGL_CORE_SHADER_H_
#define _MYGL_CORE_SHADER_H_

#include "Vertex.h"
#include "Texture.h"

namespace MyGL {
	class Shader {
	public:
		virtual ~Shader() {}
		virtual Vertex VertexFunc(const Vertex& vertex);
		virtual Color FragmentFunc(weak_ptr<Texture> texture, const Fragment &fragment);
	};
}

#endif