#ifndef _MYGL_CORE_LIGHT_H_
#define _MYGL_CORE_LIGHT_H_

#include "Structures/Vector.h"

namespace MyGL {
	class Light {
	public:
	private:
		Vector3 _direction;
		float _diffuse;
		float _specular;
		float _ambient;
	};
}

#endif