#ifndef _MYGL_MATH_H_
#define _MYGL_MATH_H_

namespace MyGL {
	template<typename T>
	inline T clamp(T v, T min, T max) {
		if (v < min) {
			return min;
		}
		else if (v > max) {
			return max;
		}
		else {
			return v;
		}
	}
}

#endif