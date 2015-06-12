#ifndef _MYGL_CORE_EXCEPTION_
#define _MYGL_CORE_EXCEPTION_

namespace MyGL {
	class Exception : public std::exception {
	public:
		enum Level {
			kWarning = 1,
			kError = 10,
			kSevere = 100
		};
		Exception(int level = kError) : exception(), _level(level) {}
		explicit Exception(string error, int level = kError) : exception(error.c_str()), _level(level) {}

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
		int _level;
	};
}

#endif