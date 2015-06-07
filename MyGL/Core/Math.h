#ifndef _MYGL_CORE_MATH_H_
#define _MYGL_CORE_MATH_H_

#include <cmath>

namespace MyGL {
	class Math {
	public:
		static const float kEpsilon;
		static const float kEqualEpsilon;

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
		template<typename T>
		static inline T Max(T a, T b) {
			return a > b ? a : b;
		}
		template<typename T>
		static inline T Min(T a, T b) {
			return a < b ? a : b;
		}

		static inline bool Equal(float lhs, float rhs) {
			return abs(lhs - rhs) < kEqualEpsilon;
		}
		static inline bool IsZero(float val) {
			return abs(val) < kEqualEpsilon;
		}
		static inline bool Greater(float lhs, float rhs) {
			return lhs - rhs > kEqualEpsilon;
		}
		static inline bool Less(float lhs, float rhs) {
			return rhs - lhs > kEqualEpsilon;
		}
		static inline bool GreaterOrEqual(float lhs, float rhs) {
			return lhs - rhs > -kEqualEpsilon;
		}
		static inline bool LessOrEqual(float lhs, float rhs) {
			return rhs - lhs > -kEqualEpsilon;
		}

		static inline float Abs(float val) {
			return abs(val);
		}
	};
}

#endif