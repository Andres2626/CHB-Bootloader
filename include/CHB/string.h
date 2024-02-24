
#ifndef _STRING_H_
#define _STRING_H_ 1

#include <CHB/types.h>

typedef unsigned int size_t;
#define NULL ((void*)0)

int memcmp(const void* s1, const void* s2, size_t n);
void* memcpy(void *s1, const void *s2, size_t n);
int strncmp(const char *s1, const char *s2, size_t n);
size_t strlen(const char *s);
char* strstr(const char *s1, const char *s2);
char* strchr(const char *s, int c);
size_t strspn(char const *s1, char const *s2);
char* strpbrk(char const *s1, char const *s2);
char* strtok(char *s1, char const *s2);

#endif /* !_STRING_H_ */