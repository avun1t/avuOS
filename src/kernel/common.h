#ifndef __COMMON_H_
#define __COMMON_H_

#define HIGHER_HALF 0xC0000000

#define NULL 0

#define __va_argsize(t) \
	(((sizeof(t) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

#define va_start(ap, pN)    \
	((ap) = ((va_list) __builtin_next_arg(pN)))

#define va_end(ap)  ((void)0)

#define va_arg(ap, t)       \
	(((ap) = (ap) + __va_argsize(t)),       \
	*((t*)(void *)((ap) - __va_argsize(t))))

typedef char *va_list;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned long long int uint64_t;
typedef signed long long int int64_t;
typedef char *string;
typedef int int32_t;
typedef uint32_t size_t;
typedef unsigned int uint;

extern uint32_t krnlstart;
extern uint32_t krnlend;

struct  __attribute__((packed)) registers {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int num, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
};

void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint16_t value);
void outl(uint16_t port, uint32_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);
int sgn(int x);
int abs(float x);
void *memset(void *dest, char val, int count);
void *memcpy(void *dest, const void *src, size_t count);
void num_to_hex_string(uint8_t num, char *str);
char nibble_to_hex_string(uint8_t num);
char *itoa(int i, char *p, int base);
bool is_a_character(uint8_t num);
int strlen(const char *str);
bool strcmp(char *str1, char *str2);
int index_of(char c, char *str);
int index_of_n(char c, int n, char *str);
void substr(int i, char *src, char *dest);
void substri(int i, char *src, char *dest);
void substrr(int s, int e, char *src, char *dest);
void strcpy(char *src, char *dest);
int count_of(char c, char *str);
bool contains(char *str, char *cont);
void cli();
void sti();
int str_to_int(char *str);
void to_upper(char *str);
char *strcat(char *dest, const char *src);

#endif // __COMMON_H_