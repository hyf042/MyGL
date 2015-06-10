#include <iostream>
#include "MyGL/MyGL.h"

using namespace MyGL;

void OnDrawScene(float time) {
	auto &gl = GL::Instance();

	gl.Clear();

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

int main() {
	const float fps = 60.0f;

	try {
		SFMLContext context;
		context.CreateWindow(800, 600, "title");
		auto window = context.GetWindow();

		int frame_count = 0;
		sf::Clock fps_elapsed;
		sf::Clock timer;
		float last_time = 0;

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
				std::cout << "FPS: " << frame_count << std::endl;
				frame_count = 0;
				fps_elapsed.restart();
			}

			frame_count++;
			OnDrawScene(timer.getElapsedTime().asSeconds());

			context.SwapBuffers();
		}
	} catch (Exception &exception) {
		std::cout << exception.what() << std::endl;
	}

	return 0;
}