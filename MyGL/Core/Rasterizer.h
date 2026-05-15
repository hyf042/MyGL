#ifndef _MYGL_CORE_RASTERIZER_H_
#define _MYGL_CORE_RASTERIZER_H_

#include <functional>
#include "Vertex.h"

namespace MyGL {
	class Rasterizer {
	public:
		static void RasterizePoint(const Vertex &v, const std::function<void(const Fragment&)> &callback);
		static void RasterizeLine(const Vertex &v1, const Vertex &v2, const std::function<void(const Fragment&)> &callback);
		static void RasterizeTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3, const std::function<void(const Fragment&)> &callback);
		static void ScanHorizontal(float y, Vertex v1, Vertex v2, Vertex &leftMost, Vertex &rightMost);
	};
}

#endif