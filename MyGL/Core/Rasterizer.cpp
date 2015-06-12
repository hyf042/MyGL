#include "Rasterizer.h"

#include "GL.h"

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

		const Vector3 &p1 = v1.position;
		const Vector3 &p2 = v2.position;
		const Vector3 &p3 = v3.position;

		int minY = static_cast<int>(Math::Ceil(Math::Min(p1.y(), Math::Min(p2.y(), p3.y()))));
		int maxY = static_cast<int>(Math::Ceil(Math::Max(p1.y(), Math::Max(p2.y(), p3.y())) - 1.0f));
		float minX = Math::Min(p1.x(), Math::Min(p2.x(), p3.x())) - 1.0f;
		float maxX = Math::Max(p1.x(), Math::Max(p2.x(), p3.x())) + 1.0f;

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

			if (left == right) {
				if (Math::Less(leftMost.x(), rightMost.x())) {
					fragments.push_back(Vertex::Lerp(leftMost, rightMost, (left - leftMost.x()) / delta_x));
				}
			}
			else if (left < right) {
				for (int j = left; j <= right; j++) {
					auto v = Vertex::Lerp(leftMost, rightMost, (j - leftMost.x()) / delta_x);
					v.position = Vector3(static_cast<float>(j), y, v.position.z());
					fragments.push_back(v);
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