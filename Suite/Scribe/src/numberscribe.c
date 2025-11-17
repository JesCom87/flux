// src/numberscribe.c
#include <stdio.h>
#include "numberscribe.h"

void assemble(const char* input_file, const char* output_file) {
    FILE* in = fopen(input_file,"r");
    if(!in) { perror("Cannot open input file"); return; }
    FILE* out = fopen(output_file,"w");
    if(!out) { perror("Cannot open output file"); fclose(in); return; }

    char line[256];
    while(fgets(line,sizeof(line),in)) {
        // Minimal: copy lines for now
        fprintf(out,"%s",line);
    }

    printf("Assembled %s -> %s\n", input_file, output_file);
    fclose(in);
    fclose(out);
}
