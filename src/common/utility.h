#ifndef __AVUOS_UTILITY_H_
#define __AVUOS_UTILITY_H_

#include <common/type_traits.h>

namespace DC {
	template <typename T>
	typename remove_reference<T>::type &&move(T &&arg)
	{
		return static_cast<typename remove_reference<T>::type &&>(arg);
	}

	template<typename T> void swap(T &t1, T& t2)
	{
		T temp = DC::move(t1);
		t1 = DC::move(t2);
		t2 = DC::move(temp);
	}
}

#endif // __AVUOS_UTILITY_H_