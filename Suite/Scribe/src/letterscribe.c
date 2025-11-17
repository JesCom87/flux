// src/letterscribe.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "letterscribe.h"

// Simple tokenizer: splits by spaces, ignores newlines
void tokenize(const char* code) {
    char buffer[256];
    int i = 0, j = 0;
    while(code[i] != '\0') {
        if(code[i] == ' ' || code[i] == '\n' || code[i] == '\t') {
            if(j > 0) {
                buffer[j] = '\0';
                printf("TOKEN: %s\n", buffer);
                j = 0;
            }
        } else {
            buffer[j++] = code[i];
        }
        i++;
    }
    if(j > 0) {
        buffer[j] = '\0';
        printf("TOKEN: %s\n", buffer);
    }
}

// Read file and tokenize
void parse_file(const char* filename) {
    FILE* f = fopen(filename, "r");
    if(!f) {
        perror("Error opening file");
        return;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* code = malloc(size+1);
    fread(code,1,size,f);
    code[size] = '\0';
    fclose(f);

    tokenize(code);
    free(code);
}
