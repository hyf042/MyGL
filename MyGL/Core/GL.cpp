#include "GL.h"
#include "Common.h"
#include "DrawCall.h"
#include "Shader.h"

namespace MyGL {
	void GL::Init(int width, int height) {
		_shader = make_unique<Shader>();
		_colorBuffer = make_shared<ColorBuffer>(width, height);
		_depthBuffer = make_shared<Buffer<float>>(width, height);
		Viewport(0, 0, static_cast<float>(width), static_cast<float>(height));
	}

	void GL::Flush() {
		for (auto &drawCall : _drawCalls) {
			_DoDrawCall(drawCall);
		}
		_drawCalls.clear();
	}

	void GL::Clear(bool colorBuffer, bool depthBuffer, bool stencilBuffer) {
		if (colorBuffer) {
			ClearColor(Colors::Black);
		}
		if (depthBuffer) {
			ClearDepth(1.0f);
		}
		if (stencilBuffer) {
			ClearStencil();
		}
	}

	void GL::ClearColor(Color color) {
		_colorBuffer->Clear(color);
	}

	void GL::ClearDepth(float val) {
		_depthBuffer->Clear(val);
	}

	weak_ptr<const Buffer<uint32>> GL::GetBuffer() {
		return _colorBuffer;
	}

	void GL::_DoDrawCall(const unique_ptr<DrawCall> &drawCall) {
		for (auto &primitive : drawCall->PullPrimitives()) {
			_DoVertexPipeline(primitive);
			auto fragments = primitive->Rasterize();
			for (auto &fragment : fragments) {
				int x = static_cast<int>(fragment.x());
				int y = static_cast<int>(fragment.y());
				if (Math::InRange(x, 0, width() - 1) && Math::InRange(y, 0, height() - 1)) {
					_DoFragmentPipeline(x, y, fragment);
				}
			}
		}
	}

	/**
	 * Rendering Pipeline
	 */
	void GL::_DoVertexPipeline(const unique_ptr<Primitives> &primitives) {
		for (auto& vertex : primitives->get_vertices()) {
			vertex = _shader->VertexFunc(vertex);
		}
	}

	void GL::_DoFragmentPipeline(int x, int y, const Fragment &fragment) {
		// Depth test
		if (IsEnable(GL_DEPTH_TEST)) {
			if (_depthBuffer->Get(x, y) < fragment.z()) {
				return;
			}
			_depthBuffer->SetData(x, y, fragment.z());
		}

		// Generate color
		auto color = _GenerateVertexColor(fragment);

		// Alpha blend
		if (IsEnable(GL_BLEND)) {
			color = _DoAlphaBlend(color, _colorBuffer->Get(x, y), _state.srcBlendMode, _state.dstBlendMode);
		}

		// Set final color
		_colorBuffer->SetData(x, y, color);
	}

	Color GL::_DoAlphaBlend(Color src, Color dst, BlendMode srcBlendMode, BlendMode dstBlendMode) {
		Vector4 src_factor = GetBlendFactor(srcBlendMode, src, dst);
		Vector4 dst_factor = GetBlendFactor(dstBlendMode, src, dst);
		auto color = src * src_factor + dst * dst_factor;
		color.Normalize();
		return color;
	}

	Color GL::_GenerateVertexColor(const Fragment &fragment) {
		if (_state.textureTargets.find(GL_TEXTURE_2D) != _state.textureTargets.end()) {
			auto& activeTexture = _state.textureTargets[GL_TEXTURE_2D];
			if (activeTexture->width() > 0 && activeTexture->height() > 0) {
				return _shader->FragmentFunc(activeTexture, fragment);
			}
		}
		return _shader->FragmentFunc(weak_ptr<Texture>(), fragment);
	}

	/**
	 * Utilities
	 */
	Color GL::GetTexture2D(weak_ptr<Texture> texture, const Vector2 &uv) {
		if (!texture.lock()) {
			return Colors::White;
		}
		return texture.lock()->GetColor(uv);
	}

	Vector4 GL::GetBlendFactor(BlendMode blendMode, Color src, Color dst) {
		switch (blendMode) {
		case GL_ZERO:
			return Vector4(0);
		case GL_ONE:
			return Vector4(1);
		case GL_SRC_ALPHA:
			return Vector4(src.a());
		case GL_DST_ALPHA:
			return Vector4(dst.a());
		case GL_ONE_MINUS_SRC_ALPHA:
			return Vector4(1.0f - src.a());
		case GL_ONE_MINUS_DST_ALPHA:
			return Vector4(1.0f - dst.a());
		case GL_SRC_COLOR:
			return Vector4(src.r(), src.g(), src.b(), src.a());
		case GL_DST_COLOR:
			return Vector4(dst.r(), dst.g(), dst.b(), dst.a());
		case GL_ONE_MINUS_SRC_COLOR:
			return Vector4(1.0f - src.r(), 1.0f - src.g(), 1.0f - src.b(), 1.0f - src.a());
		case GL_ONE_MINUS_DST_COLOR:
			return Vector4(1.0f - dst.r(), 1.0f - dst.g(), 1.0f - dst.b(), 1.0f - src.a());
		default:
			throw Exception::NotImplementationException();
		}
	}

	bool GL::TestZCulling(const Vertex &v1, const Vertex &v2, const Vertex &v3) {
		if (!IsEnable(GL_DEPTH_TEST) || !IsEnable(GL_Z_CULLING)) {
			// no culling test
			return true;
		}
		//TODO(yifengh): not implement yet.
		return true;
	}
}