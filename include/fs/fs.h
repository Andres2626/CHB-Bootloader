
#ifndef _FS_H_
#define _FS_H_ 1

#include <sys/disk.h>

#include "types.h"

#define MAX_MOUNT_POINTS 25
#define MAX_FILES_OPENED 2
#define MAX_FS 4

struct fs_ops {
    char *name;
    int ident; /* ***_FS_IDENT definitions (check fat.c) */
    int (*mount)(struct device *disk);
    void (*unmount)();
	int (*open)(_CONST char *path);
    i32t (*read)(void *buff, u32t count);
    int (*close)();
};

#endif /* !_FS_H_ */