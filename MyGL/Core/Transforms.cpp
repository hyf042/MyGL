#include "Transforms.h"

namespace MyGL {
	Matrix4 Transforms::CreateTranslate(float x, float y, float z) {
		return Matrixs::CreateMatrix4x4(
			1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1);
	}

	Matrix4 Transforms::CreateScale(float x, float y, float z) {
		return Matrixs::CreateMatrix4x4(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1);
	}

	Matrix4 Transforms::CreateScale(float scale) {
		return CreateScale(scale, scale, scale);
	}

	Matrix4 Transforms::CreateRotation(float angle, float x, float y, float z) {
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

	Matrix4 Transforms::CreateEuler(float angle_x, float angle_y, float angle_z) {
		return CreateRotation(angle_z, Vectors::Forward) * CreateRotation(angle_y, Vectors::Up) * CreateRotation(angle_x, Vectors::Right);
	}

	Matrix4 Transforms::CreateLookAt(Vector3 eye, Vector3 at, Vector3 up) {
		Vector3 forward = at - eye;
		Vector3 right = Cross(up, forward);
		return Matrixs::CreateMatrix4x4(
			right.x(), right.y(), right.z(), -eye.x(),
			up.x(), up.y(), up.z(), -eye.y(),
			forward.x(), forward.y(), forward.z(), -eye.z(),
			0, 0, 0, 1);
	}
}