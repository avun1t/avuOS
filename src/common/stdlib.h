#ifndef __AVUOS_STDLIB_H_
#define __AVUOS_STDLIB_H_

#include <common/cstddef.h>
#include <common/cstring.h>

#define max(a,b)	\
   ({ __typeof__ (a) _a = (a); \
	   __typeof__ (b) _b = (b); \
	 _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
	   __typeof__ (b) _b = (b); \
	 _a < _b ? _a : _b; })

int atoi(char *str);
int sgn(int x);
int abs(float x);
char nibble_to_hex_string(uint8_t num);
char *itoa(int i, char *p, int base);

#endif // __AVUOS_STDLIB_H_