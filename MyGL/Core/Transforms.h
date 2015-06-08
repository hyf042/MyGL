#ifndef _MYGL_CORE_TRANFORMS_H_H
#define _MYGL_CORE_TRANFORMS_H_H

#include "Structures/Matrix.h"

namespace MyGL {
	class Transforms {
	public:
		static Matrix4 CreateTranslate(float x, float y, float z);
		static Matrix4 CreateTranslate(Vector3 offset) {
			return CreateTranslate(offset.x(), offset.y(), offset.z());
		}
		static Matrix4 CreateScale(float x, float y, float z);
		static Matrix4 CreateScale(float scale);
		static Matrix4 CreateScale(Vector3 scale) {
			return CreateScale(scale.x(), scale.y(), scale.z());
		}
		static Matrix4 CreateRotation(float angle, float x, float y, float z);
		static Matrix4 CreateRotation(float angle, Vector3 direction) {
			return CreateRotation(angle, direction.x(), direction.y(), direction.z());
		}
		static Matrix4 CreateEuler(float angle_x, float angle_y, float angle_z);
		static Matrix4 CreateLookAt(Vector3 eye, Vector3 at, Vector3 up);
	};
}

#endif