#ifndef __VFS_H_
#define __VFS_H_

typedef struct device_t {
    uint8_t disk;
} device_t;

typedef struct file_t {
    uint32_t size;
    uint32_t sectors;
    bool isDirectory;
    uint32_t fs_id;
} file_t;

typedef struct filesystem_t {
    char *type;
    bool (*probe)(struct filesystem_t *);
    bool (*read)(file_t *, uint8_t *, struct filesystem_t *); // fn, buf, fsdata;
    bool (*read_part)(file_t *, uint8_t *, uint32_t, uint32_t, struct filesystem_t *); // fn, buf, offset, amount, fsdata
    bool (*get_file)(char *, file_t *, struct filesystem_t *);
    bool (*list_dir)(file_t *, struct filesystem_t *);
    void *fs_data;
    device_t *device;
} filesystem_t;

#endif // __VFS_H_