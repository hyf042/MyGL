#ifndef _MYGL_CORE_GL_H_
#define _MYGL_CORE_GL_H_

#include "Common.h"
#include "Vertex.h"
#include "MatrixWrapper.h"
#include "PrimitivesBuilder.h"
#include "DrawCall.h"
#include "Shader.h"

namespace MyGL {
	class GL {
	public:
		static GL& Instance() {
			static GL _instance;
			return _instance;
		}

		void Init(int width, int height);

		void Clear(bool colorBuffer = true, bool depthBuffer = true, bool stencilBuffer = false);
		void ClearColor(Color color);
		void ClearDepth(float val = 1.0f);
		void ClearStencil() { throw Exception::NotImplementationException(); }

		GL& DrawArrays(PrimitivesType mode, const vector<Vector3> &vertices, const vector<Color> &colors, const vector<Vector2> &uvs, const vector<Vector3> &normals);
		GL& DrawElements(PrimitivesType mode, const vector<Vector3> &vertices, const vector<Color> &colors, const vector<Vector2> &uvs, const vector<Vector3> &normals, const vector<uint32> elements);

		/*
		 * Matrixs and Transforms
		 **/
		void MatrixMode(MatrixMode matrixMode) {
			switch (matrixMode) {
			case GL_MODEVIEW:
				break;
			case GL_PROJECTION:
				break;
			default:
				throw Exception::NotImplementationException();
			}
		}
		void LoadIdentity() {
			_GetCurrentMatrixWrapper().LoadIdentity();
		}

		void Viewport(Rect rect, float minZ = 0.0f, float maxZ = 1.0f) {
			_state.viewportMatrix = Transforms::MakeViewport(rect, minZ, maxZ);
		}
		void Viewport(float minX, float minY, float width, float height, float minZ = 0.0f, float maxZ = 1.0f) {
			_state.viewportMatrix = Transforms::MakeViewport(minX, minY, width, height, minZ, maxZ);
		}
		void LookAt(Vector3 eye, Vector3 at, Vector3 up) {
			_state.modelViewMatrix.Multiply(Transforms::MakeLookAt(eye, at, up));
		}
		void Perspective(float fovy, float aspect, float zNear, float zFar) {
			_state.projectionMatrix = Transforms::MakePerspectiveFov(fovy, aspect, zNear, zFar);
		}
		void Ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
			_state.projectionMatrix = Transforms::MakeOrtho(left, right, bottom, top, zNear, zFar);
		}

		GL& Translate(float x, float y, float z) {
			_GetCurrentMatrixWrapper().Translate(x, y, z);
			return *this;
		}
		GL& Translate(const Vector3 &offset) {
			_GetCurrentMatrixWrapper().Translate(offset);
			return *this;
		}
		GL& Scale(float x, float y, float z) {
			_GetCurrentMatrixWrapper().Scale(x, y, z);
			return *this;
		}
		GL& Scale(float scale) {
			_GetCurrentMatrixWrapper().Scale(scale);
			return *this;
		}
		GL& Scale(const Vector3 &scale) {
			_GetCurrentMatrixWrapper().Scale(scale);
			return *this;
		}
		GL& Rotate(float angle, float x, float y, float z) {
			_GetCurrentMatrixWrapper().Rotate(angle, x, y, z);
			return *this;
		}
		GL& Rotate(float angle, Vector3 direction) {
			_GetCurrentMatrixWrapper().Rotate(angle, direction);
			return *this;
		}

		const Matrix4& GetCurrentMatrix() {
			return _state.GetCurrentMatrix().get_matrix();
		}

		/*
		 * Primitives
		 **/
		GL& Begin(PrimitivesType mode) {
			if (_primitvesBuilder) {
				throw Exception("[MyGL] you should only build one primitives at a time!", Exception::kWarning);
			}
			_primitvesBuilder = PrimitivesBuilder::Begin(mode);
			return *this;
		}
		GL& End() {
			auto primitives = _primitvesBuilder->Build();
			_primitvesBuilder.release();
			if (!primitives->ValidateVertices()) {
				primitives.release();
				throw Exception("[MyGL] the vertices just built has some problems.", Exception::kWarning);
			}
			primitives = _clipping.get()->ViewFrustumClipping(std::move(primitives), _state);
			_drawCalls.push_back(make_unique<DrawCall>(primitives));
			return *this;
		}
		GL& SetColor(float r, float g, float b, float a = 1.0f) {
			return SetColor(Color(r, g, b, a));
		}
		GL& SetColor(const Color &color) {
			_state.vertexColor = color;
			return *this;
		}
		GL& SetUV(float u, float v) {
			return SetUV(Vector2(u, v));
		}
		GL& SetUV(const Vector2 &uv) {
			_state.vertexUV = uv;
			return *this;
		}
		GL& SetTexCoord(float u, float v) {
			return SetTexCoord(Vector2(u, v));
		}
		GL& SetTexCoord(const Vector2 &uv) {
			_state.vertexUV = uv;
			return *this;
		}
		GL& SetNormal(float x, float y, float z) {
			return SetNormal(Vector3(x, y, z));
		}
		GL& SetNormal(const Vector3 &normal) {
			_state.vertexNormal = normal.Unit();
			return *this;
		}
		GL& AddVertex(float x, float y, float z) {
			return AddVertex(Vector3(x, y, z));
		}
		GL& AddVertex(const Vector3 &position) {
			Vector3 world_position;
			auto viewport_position = _state.ModelToViewport(position, world_position);
			Vector3 normal = _state.NormalToWorldSpace(_state.vertexNormal);
			_DealCreateVertex(viewport_position, world_position, _state.vertexColor, _state.vertexUV, normal);
			return *this;
		}

		/*
		 * Textures
		 **/
		shared_ptr<Texture> GenTexture() {
			return make_shared<Texture>();
		}

		vector<shared_ptr<Texture>> GenTextures(int num) {
			vector<shared_ptr<Texture>> textures(num);
			for (int i = 0; i < num; i++) {
				textures[i] = make_shared<Texture>();
			}
			return textures;
		}

		GL& BindTexture(TextureTarget target, shared_ptr<Texture> texture) {
			_state.textureTargets[target] = texture;
			return *this;
		}
		GL& TexImage2D(TextureTarget target, int width, int height, const uint8 *data) {
			_state.textureTargets[target]->SetTexture(width, height, data);
			return *this;
		}
		GL& TexWrapMode(TextureTarget target, TextureWrapMode horizMode, TextureWrapMode vertMode) {
			_state.textureTargets[target]->SetWrapMode(horizMode, vertMode);
			return *this;
		}
		GL& TexFilterMode(TextureTarget target, TextureFilterMode magfiliter, TextureFilterMode minfilter, TextureFilterMode levelfilter) {
			_state.textureTargets[target]->SetMinFilter(minfilter);
			_state.textureTargets[target]->SetMaxFilter(magfiliter);
			_state.textureTargets[target]->SetLevelFilter(levelfilter);
			return *this;
		}
		GL& TexParameter(TextureTarget target, TextureParameter parameter, int val) {
			auto& texture = _state.textureTargets[target];
			switch (parameter) {
			case GL_TEXTURE_WRAP_S:
				texture->SetHorizontalWrapMode(static_cast<TextureWrapMode>(val));
				break;
			case GL_TEXTURE_WRAP_T:
				texture->SetVerticalWrapMode(static_cast<TextureWrapMode>(val));
				break;
			case GL_TEXTURE_MIN_FILTER:
				texture->SetMinFilter(static_cast<TextureFilterMode>(val));
				break;
			case GL_TEXTURE_MAX_FILTER:
				texture->SetMaxFilter(static_cast<TextureFilterMode>(val));
				break;
			case GL_TEXTURE_LEVEL:
				texture->LoadMipmap(val);
				break;
			case GL_TEXTURE_MIN_LOD:
				texture->SetMinMipmapLOD(val);
				break;
			case GL_TEXTURE_MAX_LOD:
				texture->SetMaxMinmapLOD(val);
				break;
			}
			return *this;
		}

		/*
		 * Lighting
		 **/
		GL& Enable(LightEnum light) {
			_state.lights[light]->set_enabled(true);
			return *this;
		}
		GL& Disable(LightEnum light) {
			_state.lights[light]->set_enabled(false);
			return *this;
		}
		GL& SetLightParameter(LightEnum light, LightParameter parameter, Color color) {
			switch (parameter) {
			case GL_AMBIENT:
				_state.lights[light]->set_ambient(color);
				break;
			case GL_DIFFUSE:
				_state.lights[light]->set_diffuse(color);
				break;
			case GL_SPECULAR:
				_state.lights[light]->set_specular(color);
				break;
			default:
				throw Exception::NotImplementationException();
			}
			return *this;
		}
		GL& SetLightParameter(LightEnum light, LightParameter parameter, Vector4 position) {
			switch (parameter) {
			case GL_POSITION:
				_state.lights[light]->set_position(position);
				break;
			default:
				throw Exception::NotImplementationException();
			}
			return *this;
		}
		GL& SetLightParameter(LightEnum light, LightParameter parameter, Vector3 direction) {
			switch (parameter) {
			case GL_SPOT_DIRECTION:
				_state.lights[light]->set_spot_direction(direction);
				break;
			default:
				throw Exception::NotImplementationException();
			}
			return *this;
		}
		GL& SetLightParameter(LightEnum light, LightParameter parameter, float val) {
			switch (parameter) {
			case GL_CONSTANT_ATTENUATION:
				_state.lights[light]->set_constant_attenuation(val);
				break;
			case GL_LINEAR_ATTENUATION:
				_state.lights[light]->set_linear_attenuation(val);
				break;
			case GL_QUADRATIC_ATTENUATION:
				_state.lights[light]->set_quadratic_attenuation(val);
				break;
			case GL_SPOT_CUTOFF:
				_state.lights[light]->set_spot_cutoff(val);
				break;
			case GL_SPOT_EXPONENT:
				_state.lights[light]->set_spot_exponent(val);
				break;
			case GL_SPOT_OUTER_DELTA_ANGLE:
				_state.lights[light]->set_spot_outer_delta_angle(val);
				break;
			default:
				throw Exception::NotImplementationException();
			}
			return *this;
		}
		GL& SetMaterial(LightParameter parameter, Color color) {
			switch (parameter) {
			case GL_AMBIENT:
				_state.material.set_ambient(color);
				break;
			case GL_DIFFUSE:
				_state.material.set_diffuse(color);
				break;
			case GL_SPECULAR:
				_state.material.set_specular(color);
				break;
			case GL_AMBIENT_AND_DIFFUSE:
				_state.material.set_ambient(color);
				_state.material.set_diffuse(color);
				break;
			case GL_EMISSION:
				_state.material.set_emission(color);
			default:
				throw Exception::NotImplementationException();
			}
			return *this;
		}
		GL& SetMaterial(LightParameter parameter, float val) {
			switch (parameter) {
			case GL_SHININESS:
				_state.material.set_shininess(val);
				break;
			default:
				throw Exception::NotImplementationException();
			}
			return *this;
		}
		weak_ptr<Light> GenLight() {
			auto light = make_shared<Light>();
			_state.lights.push_back(light);
			return light;
		}
		bool RemoveLight(weak_ptr<Light> light) {
			for (auto iter = _state.lights.begin(); iter != _state.lights.end(); iter++) {
				if (iter->get() == light.lock().get()) {
					_state.lights.erase(iter);
					return true;
				}
			}
			return false;
		}
		Color GetLightingColor(const Vector3 &position, const Vector3 &normal) {
			Color color = Color(0, 0, 0, 0);
			for (auto light : _state.lights) {
				if (light->enabled()) {
					color = color + light->Calculate(position, normal, _state);
				}
			}
			color.Normalize();
			return color;
		}

		/*
		 * Flags & Settings
		 **/
		GL& CullFace(CullFaceMask cullFace) {
			_state.cullFace = cullFace;
			return *this;
		}
		GL& FrontFace(Clockwise frontFace) {
			_state.frontFace = frontFace;
			return *this;
		}
		GL& BlendFunc(BlendMode srcMode, BlendMode dstMode) {
			_state.srcBlendMode = srcMode;
			_state.dstBlendMode = dstMode;
			return *this;
		}
		GL& DepthFunc(DepthFunc depthFunc) {
			_state.depthFunc = depthFunc;
			return *this;
		}
		GL& Enable(GLFlag flag) {
			_state.flags.insert(flag);
			return *this;
		}
		GL& Disable(GLFlag flag) {
			_state.flags.erase(flag);
			return *this;
		}
		bool IsEnable(GLFlag flag) {
			return _state.flags.find(flag) != _state.flags.end();
		}

		// Test
		Vector3 TestTransform(Vector3 point) {
			return _state.ModelToViewport(point);
		}

		void Flush();
		const GLState& getState() const {
			return _state;
		}
		weak_ptr<const ColorBuffer> GetBuffer();
		weak_ptr<Clipping> GetClipping() {
			return _clipping;
		}

		inline int width() const {
			return _colorBuffer->width();
		}
		inline int height() const {
			return _colorBuffer->height();
		}

		/**
		* Utility
		*/
		static Color GetTexture2D(weak_ptr<Texture> texture, const Fragment &fragment);
		static Vector4 GetBlendFactor(BlendMode blendMode, Color src, Color dst);
		bool TestZCulling(const Vertex &v1, const Vertex &v2, const Vertex &v3);
		bool DepthTest(const Fragment &fragment);
		Vector3 TransformPointToWorld(const Vector3 &position) {
			return _state.ModelToWorldSpace(position);
		}
		Vector3 TransformNormalToWorld(const Vector3 &position) {
			return _state.NormalToWorldSpace(position);
		}
		Color GetColor(const Fragment &fragment) {
			if (IsEnable(GL_LIGHTING)) {
				// Use Phong Shading, deal with per-vertex.
				return GetLightingColor(fragment.world_position(), fragment.normal());
			}
			else {
				return fragment.color();
			}
		}

	private:
		GL() : _clipping(new Clipping()) {}
		MatrixWrapper& _GetCurrentMatrixWrapper() {
			return _state.GetCurrentMatrix();
		}
		void _DoDrawCall(const unique_ptr<DrawCall> &drawCall);

		/**
		 * Vertex Pipeline
		 */
		void _DoVertexPipeline(const unique_ptr<Primitives> &primitives);
		void _DealCreateVertex(const Vector3 &viewport_position, const Vector3 &world_position, const Color &color, const Vector2 &uv, const Vector3 &normal);

		/**
		 * Fragment Pipeline
		 */
		void _DoFragmentPipeline(const Fragment &fragment);
		Color _GenerateVertexColor(const Fragment &fragment);
		Color _DoAlphaBlend(Color src, Color dst, BlendMode srcBlendMode, BlendMode dstBlendMode);

		GLState _state;

		// all draw calls.
		vector<unique_ptr<DrawCall>> _drawCalls;

		unique_ptr<Shader> _shader;
		unique_ptr<PrimitivesBuilder> _primitvesBuilder;
		shared_ptr<Clipping> _clipping;
		shared_ptr<ColorBuffer> _colorBuffer;
		shared_ptr<Buffer<float>> _depthBuffer;
		shared_ptr<Buffer<uint8>> _stencilBuffer;
	};
}

#endif