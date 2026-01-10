#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_ 1

#include "shared/shared.h"
#include "sys/stdbool.h"

#ifndef STAGE1
#define MAX_FILESYSTEMS 32
#else
#define MAX_FILESYSTEMS 1 /* in stage1 only use 1 filesystem */
#endif

typedef struct __FILE {
	char name[32];
	uint32_t flags;
	uint32_t len;
	uint32_t id;
	uint32_t eof;
	uint32_t position;
	uint32_t cluster; /* FAT filesystem */
	uint32_t device; /* filesystem identifier */
} FILE;

typedef struct fsys_entry {
	char* identifier;
	void (*init_func)    ();
	void (*mount_func)   ();
	void (*unmount_func) ();
	FILE (*dir_func)     (const char* dir);
	void (*read_func)    (FILE* file, char* buff, int len);
	FILE (*open_func)    (const char* file);
	void (*close_func)   ();
}fs_entry;

/* filesystem driver */
void init_fsys();
void mount();
void unmount();
FILE dir(const char* dir);
void read(FILE* file, char* buff, int len);
FILE open(const char* file);
void close();


#endif