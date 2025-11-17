// src/crossscribe.c
#include <stdio.h>
#include "crossscribe.h"

void link_files(const char* files[], int count) {
    printf("Linking %d file(s)...\n", count);
    for(int i=0;i<count;i++) {
        printf("Processing: %s\n", files[i]);
    }
    printf("Linked output -> linked.flux\n");
}
