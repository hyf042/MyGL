#ifndef _MYGL_CORE_RASTERIZER_H_
#define _MYGL_CORE_RASTERIZER_H_

#include "Vertex.h"

namespace MyGL {
	class Rasterizer {
	public:
		static vector<Fragment> RasterizePoint(const Vertex &v);
		static vector<Fragment> RasterizeLine(const Vertex &v1, const Vertex &v2);
		static vector<Fragment> RasterizeTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3);
		static void ScanHorizontal(float y, Vertex v1, Vertex v2, Vertex &leftMost, Vertex &rightMost);
	};
}

#endif