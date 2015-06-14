#ifndef _MYGL_EXTENSION_MESH_H_
#define _MYGL_EXTENSION_MESH_H_

#include "../Core/GL.h"

namespace MyGL {
	class Mesh/* : IClonable<Mesh> */{
	public:
		Mesh(const vector<Vector3> &vertices, const vector<Color> &colors, const vector<Vector2> &uvs, const vector<Vector3> &normals, const vector<uint32> &elements) {
			_Init(vertices, colors, uvs, normals, elements);
		}

		void Draw() {
			if (UseIndex()) {
				GL::Instance().DrawElements(GL_TRIANGLES, _vertices, _colors, _uvs, _normals, _elements);
			}
			else {
				GL::Instance().DrawArrays(GL_TRIANGLES, _vertices, _colors, _uvs, _normals);
			}
		}

		bool UseIndex() const {
			return _elements.size() > 0;
		}
	private:
		void _Init(const vector<Vector3> &vertices, const vector<Color> &colors, const vector<Vector2> &uvs, const vector<Vector3> &normals, const vector<uint32> &elements) {
			assert(vertices.size() == colors.size() && colors.size() == uvs.size() && uvs.size() == normals.size());

			_vertices = vertices;
			_normals = normals;
			_colors = colors;
			_uvs = uvs;
			_elements = elements;
		}

		vector<Vector3> _vertices;
		vector<Vector3> _normals;
		vector<Color> _colors;
		vector<Vector2> _uvs;
		vector<uint32> _elements;
	};


	class MeshImporter{
	public:
		static shared_ptr<Mesh> SharedCubeMesh();
		static shared_ptr<Mesh> SharedSphereMesh(uint32 slices = 20);
		static shared_ptr<Mesh> SharedSurfaceMesh();
	};
}

#endif