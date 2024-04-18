#include "io.h"
#include "filesystem.h"
#include "disk.h"
#include "print.h"


void fs_init() {
    // Initialize all sectors to not in use
    char buffer[SECTOR_SIZE];
    for (int i = 0; i < SECTOR_SIZE; i++) {
        buffer[i] = 0;
    }
    for (int i = 0; i < 10000; i++) {
        write_sector(i, buffer);
    }
}

File* fs_open(int sector) {
    // Check if the sector is in use
    char buffer[SECTOR_SIZE];
    read_sector(sector, buffer);
    if (buffer[0] == 0) {
        // The sector is not in use, so it can't be opened
        return NULL;
    }

    // The sector is in use, so return a File struct for it
    File* file = malloc(sizeof(File));
    file->sector = sector;
    file->is_directory = buffer[1]; // The second byte indicates whether this is a directory
    return file;
}

void fs_close(File* file) {
    // Just free the File struct
    free(file);
}

void fs_read(File* file, char* buffer) {
    // Read the sector into the buffer
    read_sector(file->sector, buffer);
}

void fs_write(File* file, const char* data) {
    // Write the data to the sector
    char buffer[SECTOR_SIZE];
    print_str("Writing to sector\n");   
    buffer[0] = 1; // Mark the sector as in use
    buffer[1] = file->is_directory; // The second byte indicates whether this is a directory
    print_str("Buffer initialized in fs_write\n");
    memcpy(buffer + 2, data, SECTOR_SIZE - 2);
    print_str("Data copied to buffer\n");
    write_sector(file->sector, buffer);
    print_str("Data written to sector\n");
}


File* fs_create_directory(int sector, const char* name, const char* location) {
    // Create a new File struct for the directory
    File* dir = malloc(sizeof(File));
    dir->sector = sector;
    dir->is_directory = 1;

    // Set the metadata
    strncpy(dir->metadata.name, name, sizeof(dir->metadata.name));
    strncpy(dir->metadata.location, location, sizeof(dir->metadata.location));

    // Initialize the directory with all zeros
    char buffer[SECTOR_SIZE];
    for (int i = 0; i < SECTOR_SIZE; i++) {
        buffer[i] = 0;
    }
    fs_write(dir, buffer);

    return dir;
}

File* fs_create_file(int sector, const char* name, const char* location) {
    // Create a new File struct for the file
    File* file = (File*) malloc(sizeof(File));
    if (file == NULL) {
        print_str("Failed to allocate memory for file\n");
        return NULL;
    }
    file->sector = sector;
    file->is_directory = 0;

    print_str("Creating file from fs\n");

    // Set the metadata
    strncpy(file->metadata.name, name, sizeof(file->metadata.name));
    strncpy(file->metadata.location, location, sizeof(file->metadata.location));

    print_str("File metadata set\n");

    // Initialize the file with all zeros
    char buffer[SECTOR_SIZE];
    for (int i = 0; i < SECTOR_SIZE; i++) {
        buffer[i] = 0;
    }
    print_str("Buffer initialized\n");
    fs_write(file, buffer);

    print_str("File initialized\n");

    // check for possible errors
    if (file == NULL) {
        print_str("Failed to create file\n");
        return NULL;
    }

    print_str("File created\n");

    return file;
}