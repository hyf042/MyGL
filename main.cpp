#include <iostream>
#include "MyGL/MyGL.h"

using namespace MyGL;

void OnDrawScene(float deltaTime) {
	auto &gl = GL::Instance();

	gl.Clear();
	gl.LoadIdentity();

	gl.Viewport(0, 0, 800, 600, 0.0f, 1.0f);
	gl.Perspective(Math::Pi * .25f, 800.0f / 600.0f, 0.1f, 100.0f);

	gl.Scale(1 + 1 * Math::Sin(deltaTime * Math::Pi));
	gl.Translate(0, 0, 30.0f);

	gl
		.Begin(GL_TRIANGLES)
		.SetColor(Colors::Red)
		.AddVertex(0.0f, 1.0f, 0.0f)
		.SetColor(Colors::Yellow)
		.AddVertex(-1.0f, -1.0f, 0.0f)
		.SetColor(Colors::Green)
		.AddVertex(1.0f, -1.0f, 0.0f)
		.End();

	//auto point = Vector3(5, 5, 80);
	//Vector3 haha = point;
	//gl.LookAt(Vector3::Zero(), Vectors::Forward, Vectors::Up);
	////GL::Instance().Perspective(Math::Pi * .25f, 800.0f / 600.0f, 0.1f, 100.0f);
	//gl.Instance().Ortho(-10, 10, -10, 10, 0.1f, 100);
	//point = gl.TestTransform(point);
	//std::cout << point << std::endl;
}

int main() {
	const float fps = 60.0f;

	try {
		SFMLContext context;
		context.CreateWindow(800, 600, "title");
		auto window = context.GetWindow();

		int frame_count = 0;
		sf::Clock elapsed;
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
			auto current_time = elapsed.getElapsedTime();
			if (current_time.asSeconds() >= 1.0f) {
				std::cout << "FPS: " << frame_count << std::endl;
				frame_count = 0;
				elapsed.restart();
			}

			frame_count++;
			OnDrawScene(current_time.asSeconds() - last_time);
			last_time = current_time.asSeconds();

			context.SwapBuffers();
		}
	} catch (Exception &exception) {
		std::cout << exception.what() << std::endl;
	}

	return 0;
}