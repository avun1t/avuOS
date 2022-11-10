#ifndef __AVUOS_STDLIB_H_
#define __AVUOS_STDLIB_H_

#include <common/cstddef.h>
#include <common/cstring.h>

int atoi(char *str);
int sgn(int x);
int abs(float x);
char nibble_to_hex_string(uint8_t num);
char *itoa(int i, char *p, int base);

#endif // __AVUOS_STDLIB_H_