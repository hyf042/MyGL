#include <iostream>
#include "MyGL/MyGL.h"

using namespace MyGL;

int main() {
	SFMLContext context;
	context.CreateWindow(800, 600, "title");
	auto window = context.GetWindow();

	auto point = Vector3(5, 5, 80);
	Vector3 haha = point;
	GL::Instance().LookAt(Vector3::Zero(), Vectors::Forward, Vectors::Up);
	//GL::Instance().Perspective(Math::Pi * .25f, 800.0f / 600.0f, 0.1f, 100.0f);
	GL::Instance().Ortho(-10, 10, -10, 10, 0.1f, 100);
	point = GL::Instance().TestTransform(point);
	std::cout << point << std::endl;

	while (window.lock()->isOpen())
	{
		sf::Event event;
		while (window.lock()->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.lock()->close();
		}

		GL::Instance().Clear();
		GL::Instance().LoadIdentity();

		context.SwapBuffers();
	}

	return 0;
}