#ifndef DISK_H
#define DISK_H

#include <stddef.h>
#include <stdint.h>

#define SECTOR_SIZE 512

void wait_disk(void);
void read_sector(int sector, char* buffer);
void write_sector(int sector, char* buffer);

#endif