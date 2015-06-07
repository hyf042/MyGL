#include <iostream>
#include "MyGL/MyGL.h"

using namespace MyGL;

int main() {
	Matrix3x3 matrix;
	Vector3 v1(1, 1, 1);
	Vector3 v2(2, 2, 2);
	auto other = v1 + v2;
	//Matrix4 matrix2(matrix);
	//auto matrix3 = matrix * matrix2;
	//std::cout << (matrix == matrix2) << std::endl;

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