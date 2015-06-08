#ifndef _MYGL_CORE_EXCEPTION_
#define _MYGL_CORE_EXCEPTION_

namespace MyGL {
	class Exception : public std::exception {
	public:
		Exception() : exception() {}
		explicit Exception(string error) : exception(error.c_str()) {}

		static Exception IndexOutOfRangeException() {
			return Exception("[MyGL] index out of range!");
		}

		static Exception NotImplementationException() {
			return Exception("[MyGL] this is not implemention");
		}
	};
}

#endif