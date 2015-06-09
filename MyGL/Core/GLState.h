#ifndef _MYGL_CORE_GLSTATE_H_
#define _MYGL_CORE_GLSTATE_H_

#include "Consts.h"
#include "MatrixWrapper.h"

namespace MyGL {
	// Restores the state of MyGL, includes all settings, matrixs, etc.
	class GLState {
	public:
		// matrix transfroms point from model space to camera space.
		MatrixWrapper modelViewMatrix;
		// matrix transforms point from camera space to projection space.
		MatrixWrapper projectionMatrix;
		// matrix transforms point from clip space to viewport space.
		Matrix4 viewportMatrix;
		// current matrix mode.
		MatrixMode matrixMode;

		GLState() : matrixMode(GL_MODEVIEW) {}

		void LoadIdentity() {
			modelViewMatrix.LoadIdentity();
			projectionMatrix.LoadIdentity();
			viewportMatrix = Matrix4::Identity();
		}

		MatrixWrapper& GetCurrentMatrix() {
			return GetMatrix(matrixMode);
		}
		MatrixWrapper& GetMatrix(MatrixMode matrixMode) {
			switch (matrixMode) {
			case GL_MODEVIEW:
				return modelViewMatrix;
			case GL_PROJECTION:
				return projectionMatrix;
			default:
				throw Exception::NotImplementationException();
			}
		}

		Vector3 ModelToViewport(Vector3 point) {
			Vector4 vec = projectionMatrix.get_matrix() * modelViewMatrix.get_matrix() * point;
			return viewportMatrix * vec;
		}
		Vector3 ModelToCameraSpace(Vector3 point) {
			return modelViewMatrix.get_matrix() * point;
		}
		Vector3 ModelToProjectionSpace(Vector3 point) {
			return projectionMatrix.get_matrix() * modelViewMatrix.get_matrix() * point;
		}
	};

}

#endif