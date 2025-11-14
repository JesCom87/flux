#include <stdio.h>
#include <stdlib.h>
#include "letterscribe.h"
#include "crossscribe.h"
#include "numberscribe.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Usage: %s <flux_source_file>\n", argv[0]);
        return 1;
    }

    const char* input_file = argv[1];

    printf("=== LetterScribe Stage ===\n");
    parse_file(input_file);

    printf("\n=== CrossScribe Stage ===\n");
    const char* files[] = { input_file };
    link_files(files, 1);

    printf("\n=== NumberScribe Stage ===\n");
    assemble(input_file, "output.flux");

    printf("\nFlux compilation finished: output.flux\n");
    return 0;
}
