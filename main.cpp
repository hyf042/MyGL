#include <iostream>
#include <SFML/Graphics.hpp>

const unsigned int Width = 200;
const unsigned int Height = 200;

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "MyGL");
	
	sf::Uint8* pixels = new sf::Uint8[Width * Height * 4];
	for (int i = 0; i < Width; i++) {
		for (int j = 0; j < Height; j++) {
			unsigned int offset = (j * Width + i) << 2;
			pixels[offset + 0] = 255;
			pixels[offset + 1] = 0;
			pixels[offset + 2] = 0;
			pixels[offset + 3] = 255;
		}
	}

	sf::Texture texture;
	if (!texture.create(Width, Height)) {
		std::cout << "error to creature texture" << std::endl;
		return -1;
	}
	texture.update(pixels);
	sf::Sprite sprite;
	sprite.setTexture(texture);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(sprite);
		window.display();
	}

	return 0;
}