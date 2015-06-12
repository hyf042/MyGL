#ifndef _MYGL_CORE_MATH_H_
#define _MYGL_CORE_MATH_H_

#include <cmath>
#include <utility>

namespace MyGL {
	class Math {
	public:
		static const float kEpsilon;
		static const float kEqualEpsilon;
		static const float Pi;
		static const float E;

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
		static inline bool InRange(float val, float left, float right) {
			if (left > right) {
				std::swap(left, right);
			}
			return GreaterOrEqual(val, left) && LessOrEqual(val, right);
		}
		static inline bool InRange(int val, int left, int right) {
			return val >= left && val <= right;
		}

		static inline float Ceil(float val) {
			return ceilf(val);
		}
		static inline float Floor(float val) {
			return floorf(val);
		}
		static inline float Round(float val) {
			return roundf(val);
		}
		static inline float Abs(float val) {
			return abs(val);
		}
		static inline float Sin(float angle) {
			return sinf(angle);
		}
		static inline float Cos(float angle) {
			return cosf(angle);
		}
		static inline float Tan(float angle) {
			return tanf(angle);
		}
		static inline float Cot(float angle) {
			return 1 / tanf(angle);
		}
		static inline float Power(float base, float exponent) {
			return powf(base, exponent);
		}
		static inline float Power(float exponent) {
			return expf(exponent);
		}
		static inline float Degree2Radian(float degree) {
			return degree * Pi / 180.0f;
		}
		static inline float Radian2Degree(float radian) {
			return radian * 180.0f / Pi;
		}

		static inline float Lerp(float from, float to, float ratio) {
			return from + (to - from) * ratio;
		}
	};
}

#endif