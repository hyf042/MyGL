#include <iostream>
#include "MyGL/MyGL.h"

using namespace MyGL;

int main() {
	SFMLContext context;
	context.CreateWindow(800, 600, "title");
	auto window = context.GetWindow();

	auto point = Vector3::Zero();
	GL::Instance().Translate(10, 0, 0);
	GL::Instance().Scale(0.5f);
	GL::Instance().Rotate(Math::Pi * .5f, Vectors::Forward);
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

		context.SwapBuffers();
	}

	return 0;
}