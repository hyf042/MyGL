#include <iostream>
#include "MyGL/MyGL.h"

using namespace MyGL;

int main()
{
	SFMLContext context;
	context.CreateWindow(800, 600, "title");
	auto window = context.GetWindow();

	while (window.lock()->isOpen())
	{
		sf::Event event;
		while (window.lock()->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.lock()->close();
		}

		context.SwapBuffers();
	}

	return 0;
}