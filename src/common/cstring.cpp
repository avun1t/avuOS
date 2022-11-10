#include "cstring.h"

char *strcat(char *dest, const char *src)
{
	uint32_t i, j;

	for (i = 0; dest[i] != '\0'; i++)
		;
	for (j = 0; src[j] != '\0'; j++)
		dest[i+j] = src[j];
	
	dest[i+j] = '\0';
	return dest;
}

bool strcmp(const char *str1, const char *str2)
{
	int i = 0;
	bool flag = false;

	while (str1[i] != '\0' && str2[i] != '\0') {
		if (str1[i] != str2[i]) {
			flag = 1;
			break;
		}
		i++;
	}

	return flag == 0 && str1[i] == '\0' && str2[i] == '\0';
}

void *memset(void *dest, char val, int count)
{
	char *temp = (char *)dest;
	for (; count != 0; count--) *temp++ = val;
	return dest;
}

void *memcpy(void *dest, const void *src, size_t count)
{
	const char *sp = (const char *)src;
	char *dp = (char *)dest;
	for (; count != 0; count--) *dp++ = *sp++;
	return dest;
}

int strlen(const char *str)
{
	const char *s;

	for (s = str; *s; ++s)
		;
	return (s - str);
}

void substr(int i, char *src, char *dest)
{
	memcpy(dest,src,i);
	dest[i] = '\0';
}

void substri(int i, char *src, char *dest)
{
	memcpy(dest,src,i+1);
	dest[i+1] = '\0';
}

void substrr(int s, int e, char *src, char *dest)
{
	memcpy(dest,&src[s],e-s);
	dest[e-s] = '\0';
}

void strcpy(char *dest, const char *src)
{
	size_t length = strlen(src);
	memcpy(dest, src, length);
	dest[length] = '\0';
}