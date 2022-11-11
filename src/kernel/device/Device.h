#ifndef __DEVICE_H_
#define __DEVICE_H_

#include <kernel/filesystem/File.h>

class Device : public File {
public:
    Device();
};

#endif // __DEVICE_H_