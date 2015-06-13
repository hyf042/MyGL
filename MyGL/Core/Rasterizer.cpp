#include "Rasterizer.h"

#include "GL.h"
#include "Math.h"

namespace MyGL {
	vector<Fragment> Rasterizer::RasterizePoint(const Vertex &v) {
		return RasterizeTriangle(v, v, v);
	}

	vector<Fragment> Rasterizer::RasterizeLine(const Vertex &v1, const Vertex &v2) {
		return RasterizeTriangle(v1, v2, v2);
	}

	vector<Fragment> Rasterizer::RasterizeTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3) {
		if (GL::Instance().GetClipping().lock()->BackFaceCulling(v1, v2, v3, GL::Instance().getState())) {
			return vector<Fragment>();
		}
		if (!GL::Instance().TestZCulling(v1, v2, v3)) {
			return vector<Fragment>();
		}
		vector<Fragment> fragments;

		const Vector3 &p1 = v1.position();
		const Vector3 &p2 = v2.position();
		const Vector3 &p3 = v3.position();

		int minY = static_cast<int>(Math::Ceil(Math::Min(p1.y(), Math::Min(p2.y(), p3.y()))));
		int maxY = static_cast<int>(Math::Ceil(Math::Max(p1.y(), Math::Max(p2.y(), p3.y())) - 1.0f));
		minY = Math::Clamp(minY, 0, GL::Instance().height());
		maxY = Math::Clamp(maxY, 0, GL::Instance().height());

		float minX = Math::Min(p1.x(), Math::Min(p2.x(), p3.x())) - 1.0f;
		float maxX = Math::Max(p1.x(), Math::Max(p2.x(), p3.x())) + 1.0f;
		minX = Math::Clamp(minX, 0.0f, static_cast<float>(GL::Instance().width()));
		maxX = Math::Clamp(maxX, 0.0f, static_cast<float>(GL::Instance().width()));

		for (int i = minY; i <= maxY; i++) {
			float y = static_cast<float>(i);
			Vertex leftMost(Vector3(maxX, y, 0));
			Vertex rightMost(Vector3(minX, y, 0));
			ScanHorizontal(y, v1, v2, leftMost, rightMost);
			ScanHorizontal(y, v2, v3, leftMost, rightMost);
			ScanHorizontal(y, v3, v1, leftMost, rightMost);

			int left = static_cast<int>(Math::Ceil(leftMost.x()));
			int right = static_cast<int>(Math::Ceil(rightMost.x() - 1));
			float delta_x = rightMost.x() - leftMost.x();

			// notices the below part include the calculation of 'footprint', it's the size of the area that one pixel projects onto the texture space,
			// we need to use footprint to determine the LOD of mipmap in texture filtering, here we use a simple way to calculate it out.
			// it is, the right-pixel(lerp with x axis + 1)'s offset in x axis related to current pixel.
			if (left == right) {
				if (Math::Less(leftMost.x(), rightMost.x())) {
					Vertex last_vertex = Vertex::Lerp(leftMost, rightMost, (left + 1 - leftMost.x()) / delta_x);
					auto v = Vertex::Lerp(leftMost, rightMost, (left - leftMost.x()) / delta_x);
					v.set_position(Vector3(static_cast<float>(left), y, v.z()));
					fragments.push_back(Fragment(v, Math::Abs(last_vertex.uv().x() - v.uv().x())));
				}
			}
			else if (left < right) {
				Vertex last_vertex = Vertex::Lerp(leftMost, rightMost, (right + 1 - leftMost.x()) / delta_x);
				for (int j = right; j >= left; j--) {
					auto v = Vertex::Lerp(leftMost, rightMost, (j - leftMost.x()) / delta_x);
					v.set_position(Vector3(static_cast<float>(j), y, v.z()));
					fragments.push_back(Fragment(v, Math::Abs(last_vertex.uv().x() - v.uv().x())));
					last_vertex = v;
				}
			}
		}

		return fragments;
	}

	void Rasterizer::ScanHorizontal(float y, Vertex v1, Vertex v2, Vertex &leftMost, Vertex &rightMost) {
		if (!Math::InRange(y, v1.y(), v2.y())) {
			return;
		}

		// If the segment is horizontal.
		if (Math::Equal(v1.y(), v2.y())) {
			if (v1.x() > v2.x()) {
				std::swap(v1, v2);
			}
			if (v1.x() < leftMost.x()) {
				leftMost = v1;
			}
			if (v2.x() > rightMost.x()) {
				rightMost = v2;
			}
			return;
		}
		if (v1.y() > v2.y()) {
			std::swap(v1, v2);
		}
		Vertex lerp = Vertex::Lerp(v1, v2, (y - v1.y()) / (v2.y() - v1.y()));
		if (lerp.x() < leftMost.x()) {
			leftMost = lerp;
		}
		if (lerp.x() > rightMost.x()) {
			rightMost = lerp;
		}
	}
}