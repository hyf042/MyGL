#ifndef _SFML_CONTEXT_H_
#define _SFML_CONTEXT_H_

#include <SFML/Graphics.hpp>
#include "MyGL.h"

namespace MyGL {
	class SFMLContext : public RenderingContext {
	private:
		shared_ptr<sf::RenderWindow> m_window;
		shared_ptr<sf::Texture> m_texture;
		shared_ptr<sf::Sprite> m_sprite;
	public:
		override int width() const {
			return m_window->getSize().x;
		}

		override int height() const {
			return m_window->getSize().y;
		}

		override void createWindow(int width, int height, const string& title) {
			m_window = shared_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(width, height), title));

			m_texture = shared_ptr<sf::Texture>(new sf::Texture());
			if (!m_texture->create(width, height)) {
				throw new Exception("Error to create texture");
			}

			m_sprite = shared_ptr<sf::Sprite>(new sf::Sprite());
			m_sprite->setTexture(*m_texture);
		}

		override void flushWindow(const PixelBuffer& buffer) {
			m_texture->update(buffer.getPixelRaw());
			m_window->clear();
			m_window->draw(*m_sprite);
			m_window->display();
		}

		weak_ptr<sf::RenderWindow> getWindow() const {
			return m_window;
		}
	};
}

#endif