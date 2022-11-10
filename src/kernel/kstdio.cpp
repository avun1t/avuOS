#include <kernel/kstddef.h>
#include <kernel/kstdio.h>
#include <kernel/memory/paging.h>

int xpos = 0;
int ypos = 0;
char ccolor = 0x0f;

// Specific mode print_util should implement:
// putch_color(char c, char color);
// clear_screen();
// center_print(char c, char color); (Should provide a wrapper for center_print_base(c,color,width))

void putch(char c)
{
	if(c == '\b')
		backspace();
	else
		putch_color(c, ccolor);
}

void print_color(char* c, char color)
{
	int i = 0;

	while(c[i] != 0) {
		putch_color(c[i], color);
		i++;
	}
}

void print(char* c)
{
	print_color(c, ccolor);
}

void println_color(char* c, char color)
{
	print_color(c, color);
	print_color("\n", color);
}

void println(char* c)
{
	println_color(c,ccolor);
}

void set_color(char color)
{
	ccolor = color;
}

void center_print_base(char* c, char color, int width)
{
	if (xpos > 0) {
		print_color("\n",color);
	}

	int i = 0;
	while (c[i]) {
		i++;
	}

	if (i > width) {
		print_color(c,color);
	} else {
		if (i % 2 == 0) {
			int h = (width-i)/2;
			int j = 0;

			while (j < h) {
				putch_color(' ', color);
				j++;
			}

			print_color(c,color);
			j = 0;
			while (j < h) {
				putch_color(' ', color);
				j++;
			}
		} else {
			int h = (width-i)/2;
			int j = 0;
			while (j < h) {
				putch_color(' ', color);
				j++;
			}

			print_color(c,color);
			j = 0;
			h--;

			while (j < h+2) {
				putch_color(' ', color);
				j++;
			}
		}
	}
}

void print_hex(uint8_t num)
{
	char *str = "  ";

	num_to_hex_string(num, str);
	print("0x");
	print(str);
}

void print_hexw(uint16_t num)
{
	char *str = "  ";

	print("0x");
	num_to_hex_string(num >> 8, str);
	print(str);
	num_to_hex_string(num, str);
	print(str);
}

void print_hexl(uint32_t num)
{
	char *str = "  ";

	print("0x");
	num_to_hex_string(num >> 24, str);
	print(str);
	num_to_hex_string(num >> 16, str);
	print(str);
	num_to_hex_string(num >> 8, str);
	print(str);
	num_to_hex_string(num, str);
	print(str);
}

void print_num(int num)
{
	char str[11];

	itoa(num,str,10);
	print(str);
}

void printf(char *fmt, ...)
{
	const char *p;
	va_list argp;
	int i;
	char *s;
	char fmtbuf[256];

	va_start(argp, fmt);

	for (p = fmt; *p != '\0'; p++) {
		if (*p != '%') {
			putch(*p);
			continue;
		}

		switch (*++p) {
		case 'c':
			i = va_arg(argp, int);
			putch(i);
			break;

		case 'd':
			i = va_arg(argp, int);
			s = itoa(i, fmtbuf, 10);
			print(s);
			break;

		case 's':
			s = va_arg(argp, char *);
			print(s);
			break;

		case 'x':
			i = va_arg(argp, int);
			s = itoa(i, fmtbuf, 16);
			print(s);
			break;

		case 'X':
			i = va_arg(argp, int);
			s = itoa(i, fmtbuf, 16);
			to_upper(s);
			print(s);
			break;

		case 'b':
			i = va_arg(argp, int);
			s = itoa(i, fmtbuf, 2);
			print(s);
			break;

		case '%':
			putch('%');
			break;
		}
	}

	va_end(argp);
}

void backspace()
{
	if (xpos != 0) {
		xpos--;
		putch(' ');
		xpos--;
		update_cursor();
	}
}

void PANIC(char *error, char *msg, bool hang)
{
	clear_screen();
	set_all_color(0x9f);
	set_all_color(0x9f);

	println("Good job, you crashed it.\nAnyway, here's the details, since you probably need them.\nDon't mess it up again.\n");
	println(error);
	println(msg);

	if(hang) cli();
	while(hang);
}

uint8_t* vidmem = (uint8_t*)0xB8000; // it would be 0xb8000, but we mapped the kernel to 0xc0000000.
void putch_color(char c, char color)
{
	if (c == '\r') {
		xpos = 0;
	} else if (c == '\n') {
		xpos = 0;
		ypos++;
	} else if (c == '\t') {
		for (uint8_t i = 0; i < 5; i++) {
			putch_color(' ', color);
		}
	} else {
		int pos = (xpos+(ypos*80))*2;

		vidmem[pos] = c;
		vidmem[pos+1] = color;
		xpos++;

		if (xpos >= 80) {
			ypos++;
			xpos = 0;
		}
	}
	while (ypos >= 25) {
		scroll();
	}

	update_cursor();
}

void clear_screen()
{
	for (int y=0; y<25; y++) {
		for (int x=0; x<80; x++) {
			vidmem[(x+(y*80))*2] = ' ';
		}
	}

	xpos = 0;
	ypos = 0;
}

void set_all_color(char color)
{
	for (int y=0; y<25; y++) {
		for (int x=0; x<80; x++) {
			vidmem[(x+(y*80))*2+1] = color;
		}
	}

	set_color(color);
}

void center_print(char* c, char color)
{
	center_print_base(c, color, 80);
}

void update_cursor()
{
	uint16_t position=(ypos*80) + xpos;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(position&0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((position>>8)&0xFF));
}

void scroll()
{
	uint16_t i = 80*2;

	while (i < 80*25*2) {
		vidmem[i-(80*2)] = vidmem[i];
		i++;
	}

	i = 80*2*24;

	while (i < 80*25*2) {
		vidmem[i++] = ' ';
		vidmem[i++] = 0x07;
	}

	ypos--;
}