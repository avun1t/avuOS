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

#ifndef __STDIO_H_
#define __STDIO_H_

#include <kernel/kstddef.h>
#include <common/string.h>

#ifdef DEBUG
#define ASSERT(cond) \
	if(!(cond)) { 			\
		PANIC("Assertion failed: ", __FILE__ " at line " STR(__LINE__), true); \
	}
#else
#define ASSERT(cond)
#endif

#define SCREEN_CHAR_WIDTH 80
#define SCREEN_CHAR_HEIGHT 25

void print_color(char* c, char color);
void println_color(char* c, char color);
void putch(char c);
void print(char* c);
void println(char* c);
void set_color(char color);
void center_print_base(char* c, char color, int width);
void print_hex(uint8_t num);
void print_hexw(uint16_t num);
void print_hexl(uint32_t num);
void print_num(int num);
void printf(char *fmt, ...);
void backspace();
void PANIC(char *error, char *msg, bool hang);
void putch_color(char c, char color);
void clear_screen();
void set_all_color(char color);
void center_print(char* c, char color);
void update_cursor();
void scroll();
void set_vidmem(uint8_t *memloc);

#define SCREEN_CHAR_WIDTH 80
#define SCREEN_CHAR_HEIGHT 25

#endif // __STDIO_H_