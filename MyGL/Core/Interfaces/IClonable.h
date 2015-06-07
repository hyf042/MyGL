#ifndef _MYGL_CORE_INTERFACES_ICLONABLE_H_
#define _MYGL_CORE_INTERFACES_ICLONABLE_H_

#include "../PreDefines.h"

namespace MyGL {
	template<typename T>
	class IClonable {
	public:
		virtual shared_ptr<T> Clone() const = 0;

		virtual ~IClonable() {}
	};
}

#endif