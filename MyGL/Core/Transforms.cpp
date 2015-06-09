#include "Transforms.h"

namespace MyGL {
	Matrix4 Transforms::MakeTranslate(float x, float y, float z) {
		return Matrixs::CreateMatrix4x4(
			1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1);
	}

	Matrix4 Transforms::MakeScale(float x, float y, float z) {
		return Matrixs::CreateMatrix4x4(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1);
	}

	Matrix4 Transforms::MakeScale(float scale) {
		return MakeScale(scale, scale, scale);
	}

	Matrix4 Transforms::MakeRotation(float angle, float x, float y, float z) {
		float cos = Math::Cos(angle);
		float one_minus_cos = 1 - cos;
		float sin = Math::Sin(angle);
		float xy = x * y, xz = x * z, yz = y * z;
		return Matrixs::CreateMatrix4x4(
			cos + x * x * one_minus_cos		, xy * one_minus_cos - z * sin		, xz * one_minus_cos + y * sin	, 0,
			xy * one_minus_cos + z * sin	, cos + y * y * one_minus_cos		, yz * one_minus_cos - x * sin	, 0,
			xz * one_minus_cos - y * sin	, yz * one_minus_cos + x * sin		, cos + z * z * one_minus_cos	, 0,
			0								, 0									, 0								, 1);
	}

	Matrix4 Transforms::MakeEuler(float angle_x, float angle_y, float angle_z) {
		return MakeRotation(angle_z, Vectors::Forward) * MakeRotation(angle_y, Vectors::Up) * MakeRotation(angle_x, Vectors::Right);
	}

	// Creates a view matrix,
	//		eye: the view position,
	//		at: the view direction of eye,
	//		up: the up axis of view.
	// Notice in new base of view matrix, the back axis directs to (at - eye).
	Matrix4 Transforms::MakeLookAt(Vector3 eye, Vector3 at, Vector3 up) {
		// The camera looks at the negative direction of forward axis.
		Vector3 forward = (eye - at).Unit();
		Vector3 right = Cross(up, forward).Unit();
		return Matrixs::CreateMatrix4x4(
			right.x(), right.y(), right.z(), -eye.x(),
			up.x(), up.y(), up.z(), -eye.y(),
			forward.x(), forward.y(), forward.z(), -eye.z(),
			0, 0, 0, 1);
	}

	// Creates a perspective projection matrix, from RHS to LHS.
	//		fovy: angle of pitch, in radian,
	//		aspect: width / height,
	//		zNear: z for near plane,
	//		zFar: z for far plane.
	// Also apply the z-negative (see the third row of matrix), in order to convert right-hand to left-hand in projection space,
	// the vertex order remains for rasterization.
	Matrix4 Transforms::MakePerspectiveFov(float fovy, float aspect, float zNear, float zFar) {
		assert(zNear <= zFar);

		float cot = Math::Cot(fovy * .5f);
		float distZ = zFar - zNear;
		return Matrixs::CreateMatrix4x4(
			cot / aspect, 0, 0, 0,
			0, cot, 0, 0,
			// z-negation
			0, 0, -zFar / distZ, -zNear * zFar / distZ,
			0, 0, -1, 0);
	}

	// Creates a orthogonal projection matrix, from RHS to LHS.
	//		(left, right, bottom, top): orthogonal box,
	//		zNear: z for near plane,
	//		zFar: z for far plane.
	// Also apply the z-negation (see the third row of matrix), in order to convert right-hand to left-hand in projection space.
	Matrix4 Transforms::MakeOrtho(float left, float right, float bottom, float top, float zNear, float zFar) {
		assert(left <= right && bottom <= top && zNear <= zFar);

		float distX = right - left;
		float distY = top - bottom;
		float distZ = zFar - zNear;
		return Matrixs::CreateMatrix4x4(
			2 / distX, 0, 0, -(right + left) / distX,
			0, 2 / distY, 0, -(top + bottom) / distY,
			// z-negation
			0, 0, -1 / distZ, -zNear / distZ,
			0, 0, 0, 1);
	}

	// Creates a matrix from clip space to viewport, from LHS to RHS, the (0, 0) is at the left-top corner of window.
	Matrix4 Transforms::MakeViewport(float minX, float minY, float width, float height, float minZ, float maxZ) {
		assert(minX >= 0 && minY >= 0 && width >= 0 && height >= 0);

		float half_w = width * .5f;
		float half_h = height * .5f;

		return Matrixs::CreateMatrix4x4(
			half_w, 0, 0, half_w + minX,
			0, -half_h, 0, half_h + minY,
			0, 0, maxZ - minZ, minZ,
			0, 0, 0, 1);
	}
}