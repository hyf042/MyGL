#ifndef _MYGL_CORE_EXCEPTION_
#define _MYGL_CORE_EXCEPTION_

namespace MyGL {
	class Exception : public std::exception {
	public:
		Exception() : exception() {}
		explicit Exception(string error) : exception(error.c_str()) {}
	};
}

#endif