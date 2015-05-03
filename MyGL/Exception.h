#ifndef _MYGL_EXCEPTION_
#define _MYGL_EXCEPTION_

namespace MyGL {
	class Exception : public std::exception {
	public:
		Exception() : std::exception() {}
		Exception(string error) : std::exception(error.c_str()) {}
	};
}

#endif