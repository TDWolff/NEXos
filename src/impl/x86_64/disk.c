#include <stdint.h>
#include "disk.h"
#include "print.h"
#include "io.h"

#define ATA_PRIMARY_IO     0x1F0
#define ATA_PRIMARY_CTRL   0x3F6
#define ATA_CMD_READ       0x20
#define ATA_CMD_WRITE      0x30

void wait_disk() {
    while ((inb(ATA_PRIMARY_IO + 7) & 0x80) != 0);
    while ((inb(ATA_PRIMARY_IO + 7) & 0x08) != 0);
}

void read_sector(int sector, char* buffer) {
    outb(ATA_PRIMARY_CTRL + 6, 0); // disable interrupts
    outb(ATA_PRIMARY_IO + 2, 1); // one sector
    outb(ATA_PRIMARY_IO + 3, sector & 0xFF);
    outb(ATA_PRIMARY_IO + 4, (sector >> 8) & 0xFF);
    outb(ATA_PRIMARY_IO + 5, (sector >> 16) & 0xFF);
    outb(ATA_PRIMARY_IO + 6, 0xE0 | ((sector >> 24) & 0x0F)); // LBA mode
    outb(ATA_PRIMARY_IO + 7, ATA_CMD_READ); // command

    wait_disk();

    for (int i = 0; i < 256; i++) {
        ((uint16_t*)buffer)[i] = inw(ATA_PRIMARY_IO);
    }
}

void write_sector(int sector, char* buffer) {
    outb(ATA_PRIMARY_CTRL + 6, 0); // disable interrupts
    outb(ATA_PRIMARY_IO + 2, 1); // one sector
    outb(ATA_PRIMARY_IO + 3, sector & 0xFF);
    outb(ATA_PRIMARY_IO + 4, (sector >> 8) & 0xFF);
    outb(ATA_PRIMARY_IO + 5, (sector >> 16) & 0xFF);
    outb(ATA_PRIMARY_IO + 6, 0xE0 | ((sector >> 24) & 0x0F)); // LBA mode
    outb(ATA_PRIMARY_IO + 7, ATA_CMD_WRITE); // command

    print_str("Waiting for disk\n");

    wait_disk();

    for (int i = 0; i < 256; i++) {
        outw(ATA_PRIMARY_IO, ((uint16_t*)buffer)[i]);
    }

    print_str("Disk write complete\n");
}