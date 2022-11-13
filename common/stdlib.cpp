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

#include <kernel/kstdio.h>
#include "stdlib.h"

int atoi(char *str)
{
	int len = strlen(str);
	int ret = 0;
	for (int i = 0; i < len; i++) {
		ret = ret * 10 + (str[i] - '0');
	}
	return ret;
}

int sgn(int x)
{
	if (x>0) return 1;
	else return -1;
	return 0;
}

int abs(float x)
{
	return (int)x;
}

char nibble_to_hex_string(uint8_t num)
{
	uint8_t tmp = num & 0xF;
	if (tmp < 0xA) {
		return tmp+0x30;
	} else {
		return tmp+0x57;
	}
}

char *itoa(int i, char *p, int base)
{
	char const digit[] = "0123456789";
	int nbcount = 0;
	bool flag = 0;
	int ind;

	switch(base) {
		case 10: {
			bool neg = i < 0;
			if (neg) {
				p[0] = '-';
				p++;
				i *= -1;
			}
			int shifter = i;
			do {
				++p;
				shifter = shifter / 10;
			} while (shifter);
			*p = '\0';
			do {
				*--p = digit[i % 10];
				i = i / 10;
			} while (i);
			if (neg) p--;
		}
			break;
			//I figured out how to roll base 2 and 16 into one thing... Not sure how efficient it is though
		case 2:
		case 16:
			if (i == 0) {
				p[0] = '0';
				p[1] = '\0';
			} else {
				uint8_t shift = base == 16 ? 4 : 1;
				
				for (uint32_t a = (base == 16 ? 0xF0000000 : 0x80000000); a > 0; a = a >> shift) {
					if ((i&a) != 0 || flag) {
						nbcount++; flag = true;
					}
				}

				ind = nbcount;
				for (ind > 0; ind--;)
					p[-ind+nbcount-1] = base == 16 ? (nibble_to_hex_string((i >> (ind*4)) & 0xF)) : (((i >> ind) & 0x1) ? '1' : '0');
				p[nbcount] = '\0';
			}
			break;
	}
	return p;
}