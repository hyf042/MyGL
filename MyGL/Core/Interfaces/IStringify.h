#ifndef _MYGL_CORE_INTERFACES_ISTRINGIFY_H_
#define _MYGL_CORE_INTERFACES_ISTRINGIFY_H_

#include <iostream>
#include "../PreDefines.h"

namespace MyGL {
	using std::ostream;

	class IStringify {

	public:
		virtual string ToString() const = 0;

		virtual ~IStringify() {}
	};

	static ostream& operator<<(ostream &out, const IStringify &stringify) {
		out << stringify.ToString();
		return out;
	}
}

#endif