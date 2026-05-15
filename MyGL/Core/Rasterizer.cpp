#include "Rasterizer.h"

#include "GL.h"
#include "Math.h"

namespace MyGL {
	void Rasterizer::RasterizePoint(const Vertex &v, const std::function<void(const Fragment&)> &callback) {
		RasterizeTriangle(v, v, v, callback);
	}

	void Rasterizer::RasterizeLine(const Vertex &v1, const Vertex &v2, const std::function<void(const Fragment&)> &callback) {
		RasterizeTriangle(v1, v2, v2, callback);
	}

	void Rasterizer::RasterizeTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3, const std::function<void(const Fragment&)> &callback) {
		if (GL::Instance().GetClipping().lock()->BackFaceCulling(v1, v2, v3, GL::Instance().getState())) {
			return;
		}
		if (!GL::Instance().TestZCulling(v1, v2, v3)) {
			return;
		}

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

			int left  = static_cast<int>(Math::Ceil(leftMost.x()));
			int right = static_cast<int>(Math::Ceil(rightMost.x() - 1));
			float delta_x = rightMost.x() - leftMost.x();

			if (left > right || !Math::Less(leftMost.x(), rightMost.x())) {
				continue;
			}

			float inv_dx = 1.0f / delta_x;
			float offset = static_cast<float>(left) - leftMost.x();

			// Linear attribute steps (no perspective correction needed)
			float dz  = (rightMost.z() - leftMost.z()) * inv_dx;
			float dr  = (rightMost.color().r() - leftMost.color().r()) * inv_dx;
			float dg  = (rightMost.color().g() - leftMost.color().g()) * inv_dx;
			float db  = (rightMost.color().b() - leftMost.color().b()) * inv_dx;
			float da  = (rightMost.color().a() - leftMost.color().a()) * inv_dx;

			float dnx = (rightMost.normal().x() - leftMost.normal().x()) * inv_dx;
			float dny = (rightMost.normal().y() - leftMost.normal().y()) * inv_dx;
			float dnz = (rightMost.normal().z() - leftMost.normal().z()) * inv_dx;

			float dwx = (rightMost.world_position().x() - leftMost.world_position().x()) * inv_dx;
			float dwy = (rightMost.world_position().y() - leftMost.world_position().y()) * inv_dx;
			float dwz = (rightMost.world_position().z() - leftMost.world_position().z()) * inv_dx;

			// Perspective-correct UV: interpolate u/w, v/w, and 1/w linearly
			float left_inv_w  = 1.0f / leftMost.world_z();
			float right_inv_w = 1.0f / rightMost.world_z();
			float d_inv_w = (right_inv_w - left_inv_w) * inv_dx;

			float left_u_w  = leftMost.uv().x()  * left_inv_w;
			float left_v_w  = leftMost.uv().y()  * left_inv_w;
			float right_u_w = rightMost.uv().x() * right_inv_w;
			float right_v_w = rightMost.uv().y() * right_inv_w;
			float d_u_w = (right_u_w - left_u_w) * inv_dx;
			float d_v_w = (right_v_w - left_v_w) * inv_dx;

			// Seed current values at first pixel
			float cur_z   = leftMost.z()               + offset * dz;
			float cur_r   = leftMost.color().r()        + offset * dr;
			float cur_g   = leftMost.color().g()        + offset * dg;
			float cur_b   = leftMost.color().b()        + offset * db;
			float cur_a   = leftMost.color().a()        + offset * da;
			float cur_nx  = leftMost.normal().x()       + offset * dnx;
			float cur_ny  = leftMost.normal().y()       + offset * dny;
			float cur_nz  = leftMost.normal().z()       + offset * dnz;
			float cur_wx  = leftMost.world_position().x() + offset * dwx;
			float cur_wy  = leftMost.world_position().y() + offset * dwy;
			float cur_wz  = leftMost.world_position().z() + offset * dwz;
			float cur_inv_w = left_inv_w + offset * d_inv_w;
			float cur_u_w   = left_u_w   + offset * d_u_w;
			float cur_v_w   = left_v_w   + offset * d_v_w;

			// Seed previous UV.x for footprint calculation
			float prev_u_x = (cur_inv_w - d_inv_w > 0.0f)
				? (cur_u_w - d_u_w) / (cur_inv_w - d_inv_w)
				: (cur_inv_w > 0.0f ? cur_u_w / cur_inv_w : 0.0f);

			for (int j = left; j <= right; j++) {
				float actual_w = (cur_inv_w != 0.0f) ? 1.0f / cur_inv_w : 0.0f;
				float actual_u = cur_u_w * actual_w;
				float actual_v = cur_v_w * actual_w;
				float footprint = Math::Abs(actual_u - prev_u_x);

				Vertex v(
					Vector3(static_cast<float>(j), y, cur_z),
					Color(cur_r, cur_g, cur_b, cur_a),
					Vector2(actual_u, actual_v),
					Vector3(cur_nx, cur_ny, cur_nz),
					Vector3(cur_wx, cur_wy, cur_wz)
				);
				callback(Fragment(v, footprint));

				prev_u_x  = actual_u;
				cur_z    += dz;
				cur_r    += dr;  cur_g += dg;  cur_b += db;  cur_a += da;
				cur_nx   += dnx; cur_ny += dny; cur_nz += dnz;
				cur_wx   += dwx; cur_wy += dwy; cur_wz += dwz;
				cur_inv_w += d_inv_w;
				cur_u_w  += d_u_w;
				cur_v_w  += d_v_w;
			}
		}
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
