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
			auto viewport_position = _state.ModelToViewport(position);
			_primitvesBuilder->AddVertex(Vertex(viewport_position, _state.vertexColor, _state.vertexUV, _state.vertexNormal));
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
		static Color GetTexture2D(weak_ptr<Texture> texture, const Vector2 &uv);
		static Vector4 GetBlendFactor(BlendMode blendMode, Color src, Color dst);
		bool TestZCulling(const Vertex &v1, const Vertex &v2, const Vertex &v3);

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

		/**
		 * Fragment Pipeline
		 */
		void _DoFragmentPipeline(int x, int y, const Fragment &fragment);
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