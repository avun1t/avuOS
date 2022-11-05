int VGA_WIDTH = 320;
int VGA_HEIGHT = 200;
char VGA_MODE = 0x13;

void put_pixel(int x, int y, char color);
void set_palette(char id, char r, char g, char b);

#include "VGA_13h.c"