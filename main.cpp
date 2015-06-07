#include <iostream>
#include "MyGL/MyGL.h"

using namespace MyGL;

int main() {
	Vector4 v1(1, 1, 1);
	auto result = Matrixs::Translate(0, 10, 0) * v1;
	//Matrix4 matrix2(matrix);
	//auto matrix3 = matrix * matrix2;
	std::cout << result.x() << ' ' << result.y() << ' ' << result.z() << std::endl;

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