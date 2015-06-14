#include <iostream>
#include "MyGL/MyGL.h"

using namespace MyGL;

class TestCase {
public:
	virtual void Init(SFMLContext &context) = 0;
	virtual void OnDrawScene(float time) = 0;
	virtual void OnUpdate(float time) {}
};

class TwoMesh : public TestCase {
public:
	virtual ~TwoMesh() {}

	override void Init(SFMLContext &context) {
		GL::Instance().Disable(GL_DEPTH_TEST);
	}

	override void OnDrawScene(float time) {
		auto &gl = GL::Instance();

		gl.Clear(true, false);

		gl.Viewport(0, 0, 800, 600, 0.0f, 1.0f);
		gl.Perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

		float radian = 180.0f * time / 5.0f;

		gl.LoadIdentity();
		gl.Translate(-1.5f, 0.0f, -6.0f);
		gl.Rotate(radian, 0.0f, 1.0f, 0.0f);						// Rotate The Triangle On The Y axis ( NEW )
		gl.Begin(GL_TRIANGLES);								// Start Drawing A Triangle
		gl.SetColor(1.0f, 0.0f, 0.0f);						// Red
		gl.AddVertex(0.0f, 1.0f, 0.0f);					// Top Of Triangle (Front)
		gl.SetColor(0.0f, 1.0f, 0.0f);						// Green
		gl.AddVertex(-1.0f, -1.0f, 1.0f);					// Left Of Triangle (Front)
		gl.SetColor(0.0f, 0.0f, 1.0f);						// Blue
		gl.AddVertex(1.0f, -1.0f, 1.0f);					// Right Of Triangle (Front)
		gl.SetColor(1.0f, 0.0f, 0.0f);						// Red
		gl.AddVertex(0.0f, 1.0f, 0.0f);					// Top Of Triangle (Right)
		gl.SetColor(0.0f, 0.0f, 1.0f);						// Blue
		gl.AddVertex(1.0f, -1.0f, 1.0f);					// Left Of Triangle (Right)
		gl.SetColor(0.0f, 1.0f, 0.0f);						// Green
		gl.AddVertex(1.0f, -1.0f, -1.0f);					// Right Of Triangle (Right)
		gl.SetColor(1.0f, 0.0f, 0.0f);						// Red
		gl.AddVertex(0.0f, 1.0f, 0.0f);					// Top Of Triangle (Back)
		gl.SetColor(0.0f, 1.0f, 0.0f);						// Green
		gl.AddVertex(1.0f, -1.0f, -1.0f);					// Left Of Triangle (Back)
		gl.SetColor(0.0f, 0.0f, 1.0f);						// Blue
		gl.AddVertex(-1.0f, -1.0f, -1.0f);					// Right Of Triangle (Back)
		gl.SetColor(1.0f, 0.0f, 0.0f);						// Red
		gl.AddVertex(0.0f, 1.0f, 0.0f);					// Top Of Triangle (Left)
		gl.SetColor(0.0f, 0.0f, 1.0f);						// Blue
		gl.AddVertex(-1.0f, -1.0f, -1.0f);					// Left Of Triangle (Left)
		gl.SetColor(0.0f, 1.0f, 0.0f);						// Green
		gl.AddVertex(-1.0f, -1.0f, 1.0f);					// Right Of Triangle (Left)
		gl.End();											// Done Drawing The Pyramid

		gl.LoadIdentity();									// Reset The Current Modelview Matrix

		gl.Translate(1.5f, 0.0f, -7.0f);						// Move Right 1.5 Units And Into The Screen 7.0
		gl.Rotate(radian, 1.0f, 1.0f, 1.0f);					// Rotate The Quad On The X axis ( NEW )
		gl.Begin(GL_QUADS);									// Draw A Quad
		gl.SetColor(0.0f, 1.0f, 0.0f);						// Set The Color To Green
		gl.AddVertex(1.0f, 1.0f, -1.0f);					// Top Right Of The Quad (Top)
		gl.AddVertex(-1.0f, 1.0f, -1.0f);					// Top Left Of The Quad (Top)
		gl.AddVertex(-1.0f, 1.0f, 1.0f);					// Bottom Left Of The Quad (Top)
		gl.AddVertex(1.0f, 1.0f, 1.0f);					// Bottom Right Of The Quad (Top)
		gl.SetColor(1.0f, 0.5f, 0.0f);						// Set The Color To Orange
		gl.AddVertex(1.0f, -1.0f, 1.0f);					// Top Right Of The Quad (Bottom)
		gl.AddVertex(-1.0f, -1.0f, 1.0f);					// Top Left Of The Quad (Bottom)
		gl.AddVertex(-1.0f, -1.0f, -1.0f);					// Bottom Left Of The Quad (Bottom)
		gl.AddVertex(1.0f, -1.0f, -1.0f);					// Bottom Right Of The Quad (Bottom)
		gl.SetColor(1.0f, 0.0f, 0.0f);						// Set The Color To Red
		gl.AddVertex(1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Front)
		gl.AddVertex(-1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Front)
		gl.AddVertex(-1.0f, -1.0f, 1.0f);					// Bottom Left Of The Quad (Front)
		gl.AddVertex(1.0f, -1.0f, 1.0f);					// Bottom Right Of The Quad (Front)
		gl.SetColor(1.0f, 1.0f, 0.0f);						// Set The Color To Yellow
		gl.AddVertex(1.0f, -1.0f, -1.0f);					// Top Right Of The Quad (Back)
		gl.AddVertex(-1.0f, -1.0f, -1.0f);					// Top Left Of The Quad (Back)
		gl.AddVertex(-1.0f, 1.0f, -1.0f);					// Bottom Left Of The Quad (Back)
		gl.AddVertex(1.0f, 1.0f, -1.0f);					// Bottom Right Of The Quad (Back)
		gl.SetColor(0.0f, 0.0f, 1.0f);						// Set The Color To Blue
		gl.AddVertex(-1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Left)
		gl.AddVertex(-1.0f, 1.0f, -1.0f);					// Top Left Of The Quad (Left)
		gl.AddVertex(-1.0f, -1.0f, -1.0f);					// Bottom Left Of The Quad (Left)
		gl.AddVertex(-1.0f, -1.0f, 1.0f);					// Bottom Right Of The Quad (Left)
		gl.SetColor(1.0f, 0.0f, 1.0f);						// Set The Color To Violet
		gl.AddVertex(1.0f, 1.0f, -1.0f);					// Top Right Of The Quad (Right)
		gl.AddVertex(1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Right)
		gl.AddVertex(1.0f, -1.0f, 1.0f);					// Bottom Left Of The Quad (Right)
		gl.AddVertex(1.0f, -1.0f, -1.0f);					// Bottom Right Of The Quad (Right)
		gl.End();											// Done Drawing The Quad
	}
};

class NeheBox : public TestCase {
public:
	virtual ~NeheBox() {}

	override void Init(SFMLContext &context) {
		auto &gl = GL::Instance();
		_texture = context.LoadTexture("Images/nehe.bmp");
	}

	override void OnDrawScene(float time) {
		auto &gl = GL::Instance();

		gl.Clear();

		gl.Viewport(0, 0, 800, 600, 0.0f, 1.0f);

		gl.MatrixMode(GL_PROJECTION);
		gl.LoadIdentity();
		gl.Perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

		gl.LoadIdentity();									// Reset The View
		gl.Translate(0.0f, 0.0f, -5.0f);

		gl.Rotate(_xrot, 1.0f, 0.0f, 0.0f);
		gl.Rotate(_yrot, 0.0f, 1.0f, 0.0f);
		gl.Rotate(_zrot, 0.0f, 0.0f, 1.0f);

		gl.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		DrawCube();

		_xrot += 0.3f;
		_yrot += 0.2f;
		_zrot += 0.4f;
	}

	void DrawCube() {
		auto &gl = GL::Instance();

		gl.BindTexture(GL_TEXTURE_2D, _texture);

		gl.Begin(GL_QUADS);
		// Front Face
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, 1.0f);
		// Back Face
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, -1.0f);
		// Top Face
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(-1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, -1.0f);
		// Bottom Face
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(-1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, 1.0f);
		// Right face
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, 1.0f);
		// Left Face
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, -1.0f);
		gl.End();
	}

private:
	shared_ptr<Texture> _texture;
	float _xrot;
	float _yrot;
	float _zrot;
};

class TextureWrap : public TestCase {
public:
	virtual ~TextureWrap() {}

	override void Init(SFMLContext &context) {
		auto &gl = GL::Instance();
		_texture = context.LoadTexture("Images/nehe.bmp");
	}

	override void OnDrawScene(float time) {
		auto &gl = GL::Instance();

		gl.Clear();

		gl.Viewport(0, 0, 800, 600, 0.0f, 1.0f);

		gl.MatrixMode(GL_PROJECTION);
		gl.LoadIdentity();
		gl.Perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

		gl.LoadIdentity();									// Reset The View
		gl.Translate(0.0f, 0.0f, -5.0f);

		gl.Rotate(30.0f, 1.0f, 0.0f, 0.0f);
		gl.Rotate(20.0f, 0.0f, 1.0f, 0.0f);
		gl.Rotate(40.0f, 0.0f, 0.0f, 1.0f);

		gl.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		DrawCube();

		_xrot += 0.3f;
		_yrot += 0.2f;
		_zrot += 0.4f;
	}

	void DrawCube() {
		auto &gl = GL::Instance();

		gl.BindTexture(GL_TEXTURE_2D, _texture);
		gl.TexWrapMode(GL_TEXTURE_2D, GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT);

		gl.Begin(GL_QUADS);
		// Front Face
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, 1.0f);
		// Back Face
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, -1.0f);
		// Top Face
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(-1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, -1.0f);
		// Bottom Face
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(-1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, 1.0f);
		// Right face
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, 1.0f);
		// Left Face
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, -1.0f);
		gl.End();
	}

private:
	shared_ptr<Texture> _texture;
	float _xrot;
	float _yrot;
	float _zrot;
};

class TextureFilter : public TestCase {
public:
	virtual ~TextureFilter() {}

	override void Init(SFMLContext &context) {
		auto &gl = GL::Instance();
		_texture = context.LoadTexture("Images/nehe.bmp");
	}

	override void OnDrawScene(float time) {
		auto &gl = GL::Instance();

		gl.Clear();

		gl.Viewport(0, 0, 800, 600, 0.0f, 1.0f);

		gl.MatrixMode(GL_PROJECTION);
		gl.LoadIdentity();
		gl.Perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

		gl.LoadIdentity();									// Reset The View
		gl.Translate(0.0f, 0.0f, -20.0f);
		gl.Scale(1.0f, 1.0f, 1.0f);

		gl.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		DrawCube();
	}

	void DrawCube() {
		auto &gl = GL::Instance();

		gl.BindTexture(GL_TEXTURE_2D, _texture);
		gl.TexWrapMode(GL_TEXTURE_2D, GL_REPEAT, GL_REPEAT);
		gl.TexFilterMode(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR, GL_LINEAR);

		gl.Begin(GL_QUADS);
		// Front Face
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, 1.0f);
		// Back Face
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, -1.0f);
		// Top Face
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(-1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, -1.0f);
		// Bottom Face
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(-1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, 1.0f);
		// Right face
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, 1.0f);
		// Left Face
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, -1.0f);
		gl.End();
	}

private:
	shared_ptr<Texture> _texture;
	float _xrot;
	float _yrot;
	float _zrot;
};

class AlphaBox : public TestCase {
public:
	virtual ~AlphaBox() {}

	override void Init(SFMLContext &context) {
		auto &gl = GL::Instance();
		gl.Enable(GL_BLEND);
		gl.Disable(GL_DEPTH_TEST);
		gl.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		_texture = context.LoadTexture("Images/nehe.bmp");
	}

	override void OnDrawScene(float time) {
		auto &gl = GL::Instance();

		gl.Clear();

		gl.Viewport(0, 0, 800, 600, 0.0f, 1.0f);

		gl, MatrixMode(GL_PROJECTION);
		gl.LoadIdentity();
		gl.Perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

		gl.LoadIdentity();									// Reset The View
		gl.Translate(0.0f, 0.0f, -10.0f);

		gl.Rotate(_xrot, 1.0f, 0.0f, 0.0f);
		gl.Rotate(_yrot, 0.0f, 1.0f, 0.0f);
		gl.Rotate(_zrot, 0.0f, 0.0f, 1.0f);

		gl.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		DrawCube();

		gl.LoadIdentity();									// Reset The View
		gl.Translate(0.0f, 0.0f, -5.0f);

		gl.Rotate(_xrot, 1.0f, 0.0f, 0.0f);
		gl.Rotate(_yrot, 0.0f, 1.0f, 0.0f);
		gl.Rotate(_zrot, 0.0f, 0.0f, 1.0f);

		gl.SetColor(1.0f, 1.0f, 1.0f, 0.5f);
		DrawCube();

		_xrot += 0.3f;
		_yrot += 0.2f;
		_zrot += 0.4f;
	}

	void DrawCube() {
		auto &gl = GL::Instance();

		gl.BindTexture(GL_TEXTURE_2D, _texture);

		gl.Begin(GL_QUADS);
		// Front Face
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, 1.0f);
		// Back Face
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, -1.0f);
		// Top Face
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(-1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, -1.0f);
		// Bottom Face
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(-1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, 1.0f);
		// Right face
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, 1.0f);
		// Left Face
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, -1.0f);
		gl.End();
	}

private:
	shared_ptr<Texture> _texture;
	float _xrot;
	float _yrot;
	float _zrot;
};

class LightTest : public TestCase {
public:
	virtual ~LightTest() {}

	override void Init(SFMLContext &context) {
		auto &gl = GL::Instance();

		gl.Enable(GL_LIGHTING);
		gl.Enable(GL_LIGHT1);
		gl.SetLightParameter(GL_LIGHT1, GL_AMBIENT, Color(0.5f, 0.5f, 0.5f, 1.0f));
		gl.SetLightParameter(GL_LIGHT1, GL_DIFFUSE, Color(1.0f, 1.0f, 1.0f, 1.0f));
		gl.SetLightParameter(GL_LIGHT1, GL_POSITION, Vector4(0.0f, 0.0f, 2.0f, 1.0f));

		gl.Viewport(0, 0, 800, 600, 0.0f, 1.0f);

		gl.MatrixMode(GL_PROJECTION);
		gl.LoadIdentity();
		gl.Perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

		_texture = context.LoadTexture("Images/crate.bmp");
	}

	override void OnDrawScene(float time) {
		auto &gl = GL::Instance();

		gl.Clear();

		gl.MatrixMode(GL_MODEVIEW);
		gl.LoadIdentity();									// Reset The View
		gl.Translate(0.0f, 0.0f, _z);

		gl.Rotate(_xrot, 1.0f, 0.0f, 0.0f);
		gl.Rotate(_yrot, 0.0f, 1.0f, 0.0f);

		gl.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		DrawCube();

		_xrot += _xspeed * 5;
		_yrot += _yspeed * 5;
	}

	void DrawCube() {
		auto &gl = GL::Instance();

		gl.BindTexture(GL_TEXTURE_2D, _texture);

		gl.Begin(GL_QUADS);
		// Front Face
		gl.SetNormal(0.0f, 0.0f, 1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, 1.0f);
		// Back Face
		gl.SetNormal(0.0f, 0.0f, -1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, -1.0f);
		// Top Face
		gl.SetNormal(0.0f, 1.0f, 0.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(-1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, -1.0f);
		// Bottom Face
		gl.SetNormal(0.0f, -1.0f, 0.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(-1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, 1.0f);
		// Right face
		gl.SetNormal(1.0f, 0.0f, 0.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, -1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(1.0f, -1.0f, 1.0f);
		// Left Face
		gl.SetNormal(-1.0f, 0.0f, 0.0f);
		gl.SetTexCoord(0.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, -1.0f);
		gl.SetTexCoord(1.0f, 0.0f); gl.AddVertex(-1.0f, -1.0f, 1.0f);
		gl.SetTexCoord(1.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, 1.0f);
		gl.SetTexCoord(0.0f, 1.0f); gl.AddVertex(-1.0f, 1.0f, -1.0f);
		gl.End();
	}

	override void OnUpdate(float time) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) {
			_isLighting = !_isLighting;
			if (!_isLighting)
			{
				GL::Instance().Disable(GL_LIGHTING);
			}
			else
			{
				GL::Instance().Enable(GL_LIGHTING);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
			_xspeed -= 0.01f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
			_xspeed += 0.01f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
			_yspeed -= 0.01f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
			_yspeed += 0.01f;
		}
	}

private:
	shared_ptr<Texture> _texture;
	float _xrot;
	float _yrot;
	float _xspeed = 0;				// X Rotation Speed
	float _yspeed = 0;				// Y Rotation Speed
	bool _isLighting = true;
	float _z = -5.0f;
};

int main() {
	try {
		SFMLContext context;
		context.CreateWindow(800, 600, "title");
		auto window = context.GetWindow();

		int frame_count = 0;
		sf::Clock fps_elapsed;
		sf::Clock timer;

		unique_ptr<TestCase> testCase = make_unique<LightTest>();
		testCase->Init(context);

		while (window.lock()->isOpen())
		{
			sf::Event event;
			while (window.lock()->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.lock()->close();
			}

			// show fps
			auto current_time = fps_elapsed.getElapsedTime();
			if (current_time.asSeconds() >= 1.0f) {
				system("cls");
				std::cout << "FPS: " << frame_count << std::endl;
				frame_count = 0;
				fps_elapsed.restart();
			}

			frame_count++;
			float time = timer.getElapsedTime().asSeconds();
			testCase->OnUpdate(time);
			testCase->OnDrawScene(time);

			context.SwapBuffers();
		}
	} catch (Exception &exception) {
		std::cout << exception.what() << std::endl;
	}

	return 0;
}