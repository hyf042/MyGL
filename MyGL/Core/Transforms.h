#ifndef _MYGL_CORE_TRANFORMS_H_H
#define _MYGL_CORE_TRANFORMS_H_H

#include "Structures/Matrix.h"

namespace MyGL {
	class Transforms {
	public:
		static Matrix4 MakeTranslate(float x, float y, float z);
		static Matrix4 MakeTranslate(Vector3 offset) {
			return MakeTranslate(offset.x(), offset.y(), offset.z());
		}
		static Matrix4 MakeScale(float x, float y, float z);
		static Matrix4 MakeScale(float scale);
		static Matrix4 MakeScale(Vector3 scale) {
			return MakeScale(scale.x(), scale.y(), scale.z());
		}
		static Matrix4 MakeRotation(float angle, float x, float y, float z);
		static Matrix4 MakeRotation(float angle, Vector3 direction) {
			return MakeRotation(angle, direction.x(), direction.y(), direction.z());
		}
		static Matrix4 MakeEuler(float angle_x, float angle_y, float angle_z);
		static Matrix4 MakeLookAt(Vector3 eye, Vector3 at, Vector3 up);
		static Matrix4 MakePerspectiveFov(float fovy, float aspect, float near, float far);
		static Matrix4 MakeOrtho(Rect rect, float zNear, float zFar) {
			return MakeOrtho(rect.left(), rect.right(), rect.bottom(), rect.top(), zNear, zFar);
		}
		static Matrix4 MakeOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
		static Matrix4 MakeViewport(Rect rect, float minZ = 0.0f, float maxZ = 1.0f) {
			return MakeViewport(rect.x(), rect.y(), rect.width(), rect.height(), minZ, maxZ);
		}
		static Matrix4 MakeViewport(float minX, float minY, float width, float height, float minZ = 0.0f, float maxZ = 1.0f);
	};
}

#endif