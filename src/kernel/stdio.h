#ifndef __STDIO_H_
#define __STDIO_H_

#include <common.h>

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

#endif // __STDIO_H_