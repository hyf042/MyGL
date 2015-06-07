#ifndef _MYGL_CORE_INTERFACES_ICLONABLE_H_
#define _MYGL_CORE_INTERFACES_ICLONABLE_H_

namespace MyGL {
	template<typename T>
	class IClonable {
	public:
		virtual T Clone() const = 0;

		virtual ~IClonable() {}
	};
}

#endif