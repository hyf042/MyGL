#ifndef _MYGL_CORE_PRIMITIVES_H_
#define _MYGL_CORE_PRIMITIVES_H_

namespace MyGL {
	class Primitive {
	public:
	};

	class Point : public Primitive {};
	class Line : public Primitive {};
	class Triangle : public Primitive {};
	class Quad : public Primitive {};
}

#endif