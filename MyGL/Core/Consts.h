#ifndef _MYGL_CORE_CONSTS_H_
#define _MYGL_CORE_CONSTS_H_

namespace MyGL {
	enum MatrixMode {
		GL_MODEVIEW,
		GL_PROJECTION,
		GL_TEXTURE,
		GL_COLOR
	};

	enum PrimitivesType {
		GL_POINTS,
		GL_LINES,
		GL_LINE_LOOP,
		GL_LINE_STRIP,
		GL_TRIANGLES,
		GL_TRIANGLE_STRIP,
		GL_TRIANGLE_FAN,
		GL_QUADS,
		GL_QUAD_STRIP
	};

	enum CullFaceMask {
		GL_FACE_NONE = 0,
		GL_FRONT = 1,
		GL_BACK = 2,
		GL_FRONT_AND_BACK = 3
	};

	enum Clockwise {
		GL_CW,
		GL_CCW
	};

	enum TextureTarget {
		GL_TEXTURE_2D
	};

	enum GLFlag {
		GL_FLAG_NONE = 0,
		GL_DEPTH_TEST = 1,
		GL_BLEND = 2,
		GL_Z_CULLING = 4
	};

	enum BlendMode {
		GL_ZERO,
		GL_ONE,
		GL_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_SRC_COLOR,
		GL_DST_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_ONE_MINUS_DST_COLOR
	};
}

#endif