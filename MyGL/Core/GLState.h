#ifndef _MYGL_CORE_GLSTATE_H_
#define _MYGL_CORE_GLSTATE_H_

#include <map>
#include <set>
#include "Consts.h"
#include "MatrixWrapper.h"
#include "Texture.h"
#include "Structures/Color.h"

namespace MyGL {
	using std::map;
	using std::set;

	// Restores the state of MyGL, includes all settings, matrixs, etc.
	class GLState {
	public:
		// matrix transfroms point from model space to camera space.
		MatrixWrapper modelViewMatrix ;
		// matrix transforms point from camera space to projection space.
		MatrixWrapper projectionMatrix;
		// matrix transforms point from clip space to viewport space.
		Matrix4 viewportMatrix;
		// current matrix mode.
		MatrixMode matrixMode = GL_MODEVIEW;
		// current vertex color.
		Color vertexColor = Colors::White;
		// current vertex uv.
		Vector2 vertexUV = Vector2();
		// current vertex normal.
		Vector3 vertexNormal = Vector3();
		// mask of cull face.
		CullFaceMask cullFace = GL_BACK;
		// indicate which is front face.
		Clockwise frontFace = GL_CCW;
		// texture targets.
		map<TextureTarget, shared_ptr<Texture>> textureTargets;
		// flags.
		set<GLFlag> flags;
		// src blend mode.
		BlendMode srcBlendMode = GL_SRC_ALPHA;
		// dst blend mode.
		BlendMode dstBlendMode = GL_ONE_MINUS_SRC_ALPHA;
		// depth test func.
		DepthFunc depthFunc = GL_LESS;

		GLState() {
			flags.insert(GL_DEPTH_TEST);
		}

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

		Vector3 ModelToCameraSpace(Vector3 point) {
			return modelViewMatrix.get_matrix() * point;
		}
		Vector3 ModelToProjectionSpace(Vector3 point) {
			return projectionMatrix.get_matrix() * modelViewMatrix.get_matrix() * point;
		}
		Vector3 ModelToViewport(Vector3 point) {
			return viewportMatrix * projectionMatrix.get_matrix() * modelViewMatrix.get_matrix() * point;
		}
		Vector3 ModelToViewport(Vector3 point, Vector3 &world_position) {
			world_position = modelViewMatrix.get_matrix() * point;
			return viewportMatrix * projectionMatrix.get_matrix() * world_position;
		}
		bool CheckFlag(GLFlag flag) {
			return flags.find(flag) != flags.end();
		}
	};

}

#endif