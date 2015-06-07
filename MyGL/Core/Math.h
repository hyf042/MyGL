#ifndef _MYGL_CORE_MATH_H_
#define _MYGL_CORE_MATH_H_

namespace MyGL {
	class Math {
	public:
		template<typename T>
		static inline T Clamp(T v, T min, T max) {
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
	};
}

#endif