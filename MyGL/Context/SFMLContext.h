#ifndef _MYGL_CONTEXT_SFMLCONTEXT_H_
#define _MYGL_CONTEXT_SFMLCONTEXT_H_

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
			GL::Instance().Init(width, height);
			
			_window = make_shared<sf::RenderWindow>(sf::VideoMode(width, height), title);

			_texture = make_shared<sf::Texture>();
			if (!_texture->create(width, height)) {
				throw new Exception("Error to create texture");
			}

			_sprite = make_shared<sf::Sprite>();
			_sprite->setTexture(*_texture);
		}

		override void FlushWindow(weak_ptr<const ColorBuffer> buffer) {
			_texture->update(buffer.lock()->GetRawBytes());
			_window->clear();
			_window->draw(*_sprite);
			_window->display();
		}

		weak_ptr<sf::RenderWindow> GetWindow() const {
			return _window;
		}

		shared_ptr<Texture> LoadTexture(const string &filename) const {
			sf::Image image;
			image.loadFromFile(filename);
			image.flipVertically();
			auto texture = GL::Instance().GenTexture();
			// SFML use ABGR format, we should convert it into ARGB
			auto rawBytes = _ConvertABGRToARGB(image.getPixelsPtr(), image.getSize().x * image.getSize().y);
			texture->SetTexture(image.getSize().x, image.getSize().y, reinterpret_cast<uint8*>(&rawBytes[0]));
			return texture;
		}

	private:
		static vector<uint32> _ConvertABGRToARGB(const uint8 *bytes, int size) {
			const uint32 *dwords = reinterpret_cast<const uint32*>(bytes);
			vector<uint32> ret;
			for (int i = 0; i < size; i++) {
				ret.push_back(Color::OfABGR(dwords[i]));
			}
			return ret;
		}

		shared_ptr<sf::RenderWindow> _window;
		shared_ptr<sf::Texture> _texture;
		shared_ptr<sf::Sprite> _sprite;
	};
}

#endif