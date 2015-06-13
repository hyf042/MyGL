#ifndef _MYGL_CORE_STRUCTURES_BUFFER_H_
#define _MYGL_CORE_STRUCTURES_BUFFER_H_

#include "../PreDefines.h"
#include "../Interfaces/IClonable.h"
#include "../Math.h"

namespace MyGL {
	template<typename T>
	class Buffer : public IClonable<Buffer<T>> {
	public:
		Buffer(int width, int height) : _width(width), _height(height), _data(width * height) {}
		Buffer(int width, int height, const uint8* bytes) : _width(width), _height(height), _data(width * height) {
			SetData(bytes);
		}
		virtual ~Buffer() {}

		int width() const { return _width; }
		int height() const { return _height; }

		void Clear(uint8 byte) {
			memset(&_data[0], byte, sizeof(T) * _width * _height);
		}
		void Clear() {
			memset(&_data[0], 0, sizeof(T) * _width * _height);
		}
		void Clear(const T &fill) {
			std::fill(_data.begin(), _data.end(), fill);
		}

		T Get(int index) const {
			return _data[index];
		}
		T Get(int x, int y) const {
			return _data[GetIndex(x, y)];
		}

		void SetData(int index, const T &value) {
			_data[index] = value;
		}

		void SetData(int x, int y, const T &value) {
			SetData(GetIndex(x, y), value);
		}

		void SetData(const uint8* data) {
			memcpy(&_data[0], data, sizeof(T) * _width * _height);
		}

		int GetIndex(int x, int y) const {
			return y * width() + x;
		}
		
		void Clamp(int &x, int &y) const {
			x = Math::Clamp(x, 0, width() - 1);
			y = Math::Clamp(y, 0, height() - 1);
		}

		const uint8* GetRawBytes() const {
			if (_data.size() > 0) {
				return reinterpret_cast<const uint8*>(&_data[0]);
			}
			else {
				return nullptr;
			}
		}
		const T* GetRaw() const {
			if (_data.size() > 0) {
				return &_data[0];
			}
			else {
				return nullptr;
			}
		}

		override Buffer Clone() const {
			return Buffer(*this);
		}
		override shared_ptr<Buffer> ClonePtr() const {
			return make_shared<Buffer>(*this);
		}

	protected:
		vector<T> _data;
		int _width;
		int _height;
	};

	typedef Buffer<uint32> ColorBuffer;
}

#endif