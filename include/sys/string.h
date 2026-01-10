
#ifndef _STRING_H_
#define _STRING_H_ 1

typedef unsigned int size_t;
#define NULL ((void*)0)

void* 	memcpy(void* dest, const void* src, size_t n);
void* 	memmove(void* dest, const void* src, size_t n);
void* 	memchr(const void* buff, int ch, size_t n);
int 	memcmp(const void* s1, const void* s2, size_t n);
void* 	memset(const void*str, int c, size_t n);
size_t 		strlen(const char* str);
char* 		strcat(char* dest, const char* src);
char* 		strncat(char* dest, const char* src, size_t n);
char* 		strchr(const char* s, int c);
int 		strcmp(const char* s1, const char* s2);
int 		strncmp(const char* s1, const char* s2, size_t n);
char* 		strerror(int errnum);
size_t 		strspn(const char* s1, const char* s2);
size_t 		strcspn(const char* s1, const char* s2);
const char* strpbrk(const char* s1, const char* s2);
char* 		strstr(char* str, char* substr);
char* 		strtok(char* s1, char s2);
size_t 		strxfrm(char *dest, const char *src, size_t n);
int 		strcoll(const char* s1, const char* s2);
char* 		strrev(char* str);
char* 		strcpy(char *s1, const char* s2);

#endif /* !_STRING_H_ */
