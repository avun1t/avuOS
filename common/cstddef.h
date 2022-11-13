/*
	This file is part of avuOS.
	avuOS is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	avuOS is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with avuOS.  If not, see <https://www.gnu.org/licenses/>.
	Copyright (c) avun1t 2022. All rights reserved.
*/

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

typedef char *va_list;
typedef unsigned long long int 	uint64_t;
typedef signed long long int 	int64_t;
typedef unsigned int 			uint32_t;
typedef signed int 				int32_t;
typedef unsigned short 			uint16_t;
typedef signed short 			int16_t;
typedef unsigned char 			uint8_t;
typedef signed char 			int8_t;
typedef int 					int32_t;
typedef uint32_t 				size_t;
typedef int32_t					ssize_t;
typedef int pid_t;

#endif // __AVUOS_CSTDDEF_H_