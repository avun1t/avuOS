#ifndef __AVUOS_TYPE_TRAITS_H_
#define __AVUOS_TYPE_TRAITS_H_

namespace DC {
	template<class T> struct remove_reference			{typedef T type;};
	template<class T> struct remove_reference<T&>		{typedef T type;};
	template<class T> struct remove_reference<T&&>		{typedef T type;};
}

#endif // __AVUOS_TYPE_TRAITS_H_