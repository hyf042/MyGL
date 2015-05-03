#ifndef _MYGL_PRE_DEFINES_H_
#define _MYGL_PRE_DEFINES_H_

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <iostream>
#include <cmath>
#include <ctime>
#include <exception>

namespace MyGL {
	typedef signed char int8;
	typedef unsigned char uint8;
	typedef signed short int16;
	typedef unsigned short uint16;
	typedef signed int int32;
	typedef unsigned int uint32;
	typedef signed long long int64;
	typedef unsigned long long uint64;

	typedef unsigned char byte;
	typedef unsigned short word;
	typedef unsigned int dword;
	typedef unsigned long long qword;

	typedef std::string string;
	typedef std::wstring wstring;
	typedef wchar_t wchar;

	using std::shared_ptr;
	using std::weak_ptr;
	using std::vector;

	class Exception;

	class PixelFormat;
	class PixelBuffer;

	class MyGL;

	class RenderingContext;

	#define override virtual
}

#endif