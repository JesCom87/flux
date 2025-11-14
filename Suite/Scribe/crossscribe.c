// crossscribe.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crossscribe.h"

void link_files(const char* files[], int count) {
    printf("Linking %d files...\n", count);
    for (int i = 0; i < count; i++) {
        printf("Processing: %s\n", files[i]);
    }
    printf("Linking done. Output: linked.flux\n");
}
