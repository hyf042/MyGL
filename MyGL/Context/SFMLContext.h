#ifndef _MYGL_RENDERINGCONTEXT_SFMLCONTEXT_H_
#define _MYGL_RENDERINGCONTEXT_SFMLCONTEXT_H_

#include <SFML/Graphics.hpp>
#include "RenderingContext.h"

namespace MyGL {
	class SFMLContext : public RenderingContext {
	public:
		override int width() const {
			return _window->getSize().x;
		}

		override int height() const {
			return _window->getSize().y;
		}

		override void CreateWindow(int width, int height, const string& title) {
			_window = shared_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(width, height), title));

			_texture = shared_ptr<sf::Texture>(new sf::Texture());
			if (!_texture->create(width, height)) {
				throw new Exception("Error to create texture");
			}

			_sprite = shared_ptr<sf::Sprite>(new sf::Sprite());
			_sprite->setTexture(*_texture);
		}

		override void FlushWindow(const PixelBuffer& buffer) {
			_texture->update(buffer.GetPixelRaw());
			_window->clear();
			_window->draw(*_sprite);
			_window->display();
		}

		weak_ptr<sf::RenderWindow> GetWindow() const {
			return _window;
		}

	private:
		shared_ptr<sf::RenderWindow> _window;
		shared_ptr<sf::Texture> _texture;
		shared_ptr<sf::Sprite> _sprite;
	};
}

#endif