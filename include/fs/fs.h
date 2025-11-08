
#ifndef _FS_H_
#define _FS_H_ 1

#include <sys/disk.h>

#include "types.h"

#define MAX_MOUNT_POINTS 25
#define MAX_FILES_OPENED 10
#define MAX_FS 4

#define FAT12_TYPE 0x1

struct file;
struct fs_ops;

struct fs {
    int type;
    void *fs_data;
    struct device *fs_dev;
	struct fs_ops *ops;
};

struct fs_ops {
    char *name;
    struct fs *(*detect)(struct device *disk);
    int (*mount)(struct device *disk);
    void (*unmount)();
	struct file *(*open)(_CONST char *path);
};

struct file {
    bool dir;
	u64t size;
	u64t pos;
    
    struct fs *fs; /* internal FS driver */
};

PUBLIC int vfs_register_fs(struct fs_ops *fs);
PUBLIC struct fs *vfs_detect(struct device *disk);
PUBLIC int vfs_mount(struct fs *fs);
PUBLIC struct file *vfs_open(_CONST char *path);
PUBLIC int vfs_close(struct file *fp);

#endif /* !_FS_H_ */