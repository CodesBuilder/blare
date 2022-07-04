#ifndef __BLARE_UTIL_CONTAINER_HPP__
#define __BLARE_UTIL_CONTAINER_HPP__

#include <list>

namespace Blare {
	namespace Util {
		template <typename T, typename Allocator = std::allocator<T>>
		class ArrayList : public std::list<T, Allocator> {
		public:
			inline iterator at(size_t index) {
				size_t i = 0;
				auto it = begin();
				while (i != index) {
					if (it == end())
						throw std::out_of_range();
					it++, i++;
				}
				return it;
			}
			inline const T& operator[](size_t index) {
				return *at(index);
			}
		};
	}
}

#endif
