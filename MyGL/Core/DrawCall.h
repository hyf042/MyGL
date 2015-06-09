#ifndef _MYGL_CORE_DRAWCALL_H_
#define _MYGL_CORE_DRAWCALL_H_

#include "Primitives.h"

namespace MyGL {
	class DrawCall {
	public:
		DrawCall(unique_ptr<Primitives> &primitives) : _primitives(vector<unique_ptr<Primitives> >(0)) {
			AddPrimitives(primitives);
		}

		DrawCall(vector<unique_ptr<Primitives>> &other) {
			for (auto &primitives : other) {
				AddPrimitives(primitives);
			}
		}

		void AddPrimitives(unique_ptr<Primitives> &primitive) {
			_primitives.push_back(std::move(primitive));
		}
		vector<unique_ptr<Primitives>>& PullPrimitives() {
			return _primitives;
		}
	private:
		DrawCall(const DrawCall &other);

		vector<unique_ptr<Primitives> > _primitives;
	};
}

#endif