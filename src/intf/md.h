#ifndef MD_H
#define MD_H

#include <stddef.h>

typedef struct {
    char name[256];
    char location[1024];
} Metadata;

#endif