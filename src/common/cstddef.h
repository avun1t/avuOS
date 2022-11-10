#ifndef __AVUOS_CSTDDEF_H_
#define __AVUOS_CSTDDEF_H_

#define NULL 0

#define __va_argsiz(t)	\
	(((sizeof(t) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

#define va_start(ap, pN)	\
	((ap) = ((va_list) __builtin_next_arg(pN)))

#define va_end(ap)	((void)0)

#define va_arg(ap, t)		\
	(((ap) = (ap) + __va_argsiz(t)),	\
	*((t*)(void*)((ap) - __va_argsiz(t))))

char *va_list;
typedef unsigned signed long long int uint64_t;
typedef signed long long int int64_t;
typedef unsigned signed int uint32_t;
typedef signed int int32_t;
typedef unsigned signed short uint16_t;
typedef signed short int16_t;
typedef unsigned signed char uint8_t;
typedef signed char int8_t;
typedef char *string;
typedef int int32_t;
typedef uint32_t size_t;
typedef unsigned int uint;

#endif // __AVUOS_CSTDDEF_H_