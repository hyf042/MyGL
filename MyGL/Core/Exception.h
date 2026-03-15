#ifndef _MYGL_CORE_EXCEPTION_
#define _MYGL_CORE_EXCEPTION_

#include <exception>
#include <string>

namespace MyGL {
	class Exception : public std::exception {
	public:
		enum Level {
			kWarning = 1,
			kError = 10,
			kSevere = 100
		};
		Exception(int level = kError) : _level(level) {}
		explicit Exception(std::string error, int level = kError) : _message(error), _level(level) {}

		virtual const char* what() const noexcept { return _message.c_str(); }

		int get_level() const {
			return _level;
		}

		static Exception IndexOutOfRangeException() {
			return Exception("[MyGL] index out of range!");
		}

		static Exception NotImplementationException() {
			return Exception("[MyGL] this is not implemention!");
		}

		static Exception ArrayEmptyException() {
			return Exception("[MyGL] this array is empty!");
		}

		static Exception NotExistedException() {
			return Exception("[MyGL] not exist this item!");
		}

	private:
		std::string _message;
		int _level;
	};
}

#endif