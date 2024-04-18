#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "io.h"
#include "disk.h"
#include "mem.h"
#include "md.h"

typedef struct {
    int sector;
    int is_directory;
    Metadata metadata;
} File;

void fs_init();
File* fs_open(int sector);
void fs_close(File* file);
void fs_read(File* file, char* buffer);
void fs_write(File* file, const char* data);
File* fs_create_directory(int sector, const char* name, const char* location);
File* fs_create_file(int sector, const char* name, const char* location);

#endif