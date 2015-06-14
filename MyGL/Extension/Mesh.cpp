#include "Mesh.h"

namespace MyGL {
	void _BuildSphereVertex(float radius, float a, float b, Vector3 &vertex, Vector3 &normal){
		float sina = Math::Sin(a);
		vertex = Vector3(
			radius * sina * Math::Cos(b),
			radius * sina * Math::Sin(b),
			radius * Math::Cos(a));
		normal = vertex;
		normal.Normalize();
	}

	template<typename T>
	vector<T> _CreateFromArray(T* arr, int num) {
		vector<T> ret;
		for (int i = 0; i < num; i++) {
			ret.push_back(arr[i]);
		}
		return ret;
	}

	shared_ptr<Mesh> MeshImporter::SharedCubeMesh(){
		static shared_ptr<Mesh> mesh;
		if (!mesh){
			Vector3 v[8] = {
				Vector3(0.5f, 0.5f, 0.5f),	//0
				Vector3(-0.5f, 0.5f, 0.5f),	//1
				Vector3(-0.5f, -0.5f, 0.5f),	//2
				Vector3(0.5f, -0.5f, 0.5f),	//3
				Vector3(0.5f, -0.5f, -0.5f),	//4
				Vector3(0.5f, 0.5f, -0.5f),	//5
				Vector3(-0.5f, 0.5f, -0.5f),	//6
				Vector3(-0.5f, -0.5f, -0.5f)		//7
			};

			float half4 = 0.25f,
				half2 = 0.5f,
				half43 = 0.75f,
				half3 = 1.0f / 3,
				half32 = 2.0f / 3;


			Vector3 n[6] = {
				Vector3(0, 0, 1),//0
				Vector3(1, 0, 0),//1
				Vector3(0, 1, 0),	//2
				Vector3(-1, 0, 0),	//3
				Vector3(0, -1, 0),	//4
				Vector3(0, 0, -1),//5
			};
			vector<Vector3> normals = _CreateFromArray(n, 6);

			//vertices
			int triangleCount = 12;
			int vertexCount = 36;
			vector<Vector3> vertices(vertexCount);
			Vector3 *mv = &vertices[0];

			uint32 idx = 0;
			//front
			mv[idx++] = v[0]; mv[idx++] = v[1]; mv[idx++] = v[2];
			mv[idx++] = v[2]; mv[idx++] = v[3]; mv[idx++] = v[0];
			//right
			mv[idx++] = v[0]; mv[idx++] = v[3]; mv[idx++] = v[4];
			mv[idx++] = v[4]; mv[idx++] = v[5]; mv[idx++] = v[0];
			//top
			mv[idx++] = v[0]; mv[idx++] = v[5]; mv[idx++] = v[6];
			mv[idx++] = v[6]; mv[idx++] = v[1]; mv[idx++] = v[0];
			//left
			mv[idx++] = v[1]; mv[idx++] = v[6]; mv[idx++] = v[7];
			mv[idx++] = v[7]; mv[idx++] = v[2]; mv[idx++] = v[1];
			//bottom
			mv[idx++] = v[7]; mv[idx++] = v[4]; mv[idx++] = v[3];
			mv[idx++] = v[3]; mv[idx++] = v[2]; mv[idx++] = v[7];
			//back
			mv[idx++] = v[4]; mv[idx++] = v[7]; mv[idx++] = v[6];
			mv[idx++] = v[6]; mv[idx++] = v[5]; mv[idx++] = v[4];


			//UV
			idx = 0;

			vector<Vector2> uvs(vertexCount);
			Vector2 *muv = &uvs[0];
			muv[idx++].SetValue(1, 1); muv[idx++].SetValue(0, 1); muv[idx++].SetValue(0, 0);
			muv[idx++].SetValue(0, 0); muv[idx++].SetValue(1, 0); muv[idx++].SetValue(1, 1);

			muv[idx++].SetValue(0, 1); muv[idx++].SetValue(0, 0); muv[idx++].SetValue(1, 0);
			muv[idx++].SetValue(1, 0); muv[idx++].SetValue(1, 1); muv[idx++].SetValue(0, 1);

			muv[idx++].SetValue(1, 0); muv[idx++].SetValue(1, 1); muv[idx++].SetValue(0, 1);
			muv[idx++].SetValue(0, 1); muv[idx++].SetValue(0, 0); muv[idx++].SetValue(1, 0);

			muv[idx++].SetValue(1, 1); muv[idx++].SetValue(0, 1); muv[idx++].SetValue(0, 0);
			muv[idx++].SetValue(0, 0); muv[idx++].SetValue(1, 0); muv[idx++].SetValue(1, 1);

			muv[idx++].SetValue(0, 0); muv[idx++].SetValue(1, 0); muv[idx++].SetValue(1, 1);
			muv[idx++].SetValue(1, 1); muv[idx++].SetValue(0, 1); muv[idx++].SetValue(0, 0);

			muv[idx++].SetValue(0, 0); muv[idx++].SetValue(1, 0); muv[idx++].SetValue(1, 1);
			muv[idx++].SetValue(1, 1); muv[idx++].SetValue(0, 1); muv[idx++].SetValue(0, 0);

			//color
			vector<Color> colors(vertexCount);
			for (int i = 0; i<vertexCount; i++){
				colors[i].SetValue(1.0f, 1.0f, 1.0f);
			}

			//not use index
			vector<uint32> elements;

			mesh = make_shared<Mesh>(vertices, colors, uvs, normals, elements);
		}
		return mesh;
	}

	shared_ptr<Mesh> MeshImporter::SharedSphereMesh(uint32 slices){
		static std::map<uint32, shared_ptr<Mesh> > meshs;
		if (meshs.find(slices) == meshs.end()){
			float radius = 2.25f;

			int w = 2 * slices, h = slices;
			float hStep = 180.0f / (h - 1);
			float wStep = 360.0f / w;

			int vertexCount = h*(w + 1);
			vector<Vector3> vertices(vertexCount);
			vector<Vector3> normals(vertexCount);
			vector<Color> colors(vertexCount);
			vector<Vector2> uvs(vertexCount);

			float a, b;
			int i = 0, j = 0;
			for (a = 0.0, i = 0; i<h; i++, a += hStep){
				for (b = 0.0, j = 0; j <= w; j++, b += wStep){
					_BuildSphereVertex(radius, Math::Deg2Rad(a), Math::Deg2Rad(b), vertices[i*(w + 1) + j], normals[i*(w + 1) + j]);
					colors[i*(w + 1) + j].SetValue(1, 1, 1);
					uvs[i*(w + 1) + j].SetValue(1 - b / 360.0f, a / 180.0f);
				}
			}

			int triangleCount = (h - 1)*(w + 1) * 2;
			vector<uint32> elements(triangleCount * 3);
			uint32* p = &elements[0];
			for (i = 0; i < h - 1; i++){
				for (j = 0; j < w; j++){
					(*(p++)) = (i + 1)*(w + 1) + j + 1;
					(*(p++)) = i*(w + 1) + j + 1;
					(*(p++)) = i*(w + 1) + j;

					(*(p++)) = i*(w + 1) + j;
					(*(p++)) = (i + 1)*(w + 1) + j;
					(*(p++)) = (i + 1)*(w + 1) + j + 1;
				}

				(*(p++)) = (i + 1)*(w + 1);
				(*(p++)) = i*(w + 1);
				(*(p++)) = i*(w + 1) + j;

				(*(p++)) = i*(w + 1) + j;
				(*(p++)) = (i + 1)*(w + 1) + j;
				(*(p++)) = (i + 1)*(w + 1);

			}

			meshs[slices] = make_shared<Mesh>(vertices, colors, uvs, normals, elements);
		}
		return meshs[slices];
	}

	shared_ptr<Mesh> MeshImporter::SharedSurfaceMesh() {
		static shared_ptr<Mesh> mesh;
		if (!mesh){
			int vertexCount = 4;
			vector<Vector3> vertices(vertexCount);
			vector<Color> colors(vertexCount);
			vector<Vector2> uvs(vertexCount);
			vector<Vector3> normals(vertexCount);

			vertices[0].SetValue(-0.5f, 0.5f, 0.0f);
			vertices[1].SetValue(0.5f, 0.5f, 0.0f);
			vertices[2].SetValue(0.5f, -0.5f, 0.0f);
			vertices[3].SetValue(-0.5f, -0.5f, 0.0f);
			colors[0].SetValue(1, 1, 1, 1);
			colors[1].SetValue(1, 1, 1, 1);
			colors[2].SetValue(1, 1, 1, 1);
			colors[3].SetValue(1, 1, 1, 1);
			uvs[0].SetValue(0, 0);
			uvs[1].SetValue(1, 0);
			uvs[2].SetValue(1, 1);
			uvs[3].SetValue(0, 1);
			normals[0].SetValue(0, 1, 0);
			normals[1].SetValue(0, 1, 0);
			normals[2].SetValue(0, 1, 0);
			normals[3].SetValue(0, 1, 0);

			int triangleCount = 2;
			vector<uint32> elements(triangleCount * 3);
			elements[0] = 0;
			elements[1] = 1;
			elements[2] = 2;
			elements[3] = 3;
			elements[4] = 2;
			elements[5] = 0;

			mesh = make_shared<Mesh>(vertices, colors, uvs, normals, elements);
		}
		return mesh;
	}

}