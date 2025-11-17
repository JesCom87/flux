// flux_vm.c — simple Flux OPQ virtual machine
// compile: gcc -o flux_vm flux_vm.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STACK_SIZE 1024
static long stack[STACK_SIZE];
static int sp = 0;

void push(long v) {
    if (sp < STACK_SIZE) stack[sp++] = v;
}

long pop() {
    if (sp > 0) return stack[--sp];
    return 0;
}

// read entire file into a malloc() string
char *read_whole_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *buf = malloc(size + 1);
    if (!buf) { fclose(f); return NULL; }

    fread(buf, 1, size, f);
    buf[size] = '\0';

    fclose(f);
    return buf;
}

void runtokens(FILE *f) {
    char line[1024];

    while (fgets(line, sizeof(line), f)) {

        // skip leading whitespace
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;

        // skip blank or comment lines
        if (*p == '#' || *p == '\0' || *p == '\n') continue;

        char cmd[64];
        if (sscanf(p, "%63s", cmd) != 1)
            continue;

        /* ==========================
           CONST <num> or CONST "str"
           ========================== */
        if (strcmp(cmd, "CONST") == 0) {

            p += strlen("CONST");
            while (*p == ' ' || *p == '\t') p++;

            if (*p == '"') {
                // string literal
                char *start = strchr(p, '"');
                char *end   = start ? strchr(start + 1, '"') : NULL;

                if (start && end) {
                    *end = '\0';
                    char *s = strdup(start + 1);
                    push((long)(intptr_t)s);
                }
            } else {
                long n = atol(p);
                push(n);
            }
        }

        /* ====== PRINT ====== */
        else if (strcmp(cmd, "PRINT") == 0) {
            long v = pop();

            char *s = (char*)(intptr_t)v;

            // detect string vs number
            if (s && ((s[0] < '0' || s[0] > '9'))) {
                printf("%s\n", s);
            } else {
                printf("%ld\n", v);
            }
        }

        /* ====== ADD ====== */
        else if (strcmp(cmd, "ADD") == 0) {
            long b = pop();
            long a = pop();
            push(a + b);
        }

        /* ====== MUL ====== */
        else if (strcmp(cmd, "MUL") == 0) {
            long b = pop();
            long a = pop();
            push(a * b);
        }

        /* ====== READFILE ====== */
        else if (strcmp(cmd, "READFILE") == 0) {

            p += strlen("READFILE");
            while (*p == ' ' || *p == '\t') p++;

            if (*p == '"') {
                char *start = strchr(p, '"');
                char *end   = start ? strchr(start + 1, '"') : NULL;

                if (start && end) {
                    *end = '\0';
                    char *path = start + 1;

                    char *data = read_whole_file(path);
                    if (!data) data = strdup("");

                    push((long)(intptr_t)data);
                }
            }
        }

        /* ====== WRITEFILE ====== */
        else if (strcmp(cmd, "WRITEFILE") == 0) {

            p += strlen("WRITEFILE");
            while (*p == ' ' || *p == '\t') p++;

            if (*p == '"') {
                char *start = strchr(p, '"');
                char *end   = start ? strchr(start + 1, '"') : NULL;

                if (start && end) {
                    *end = '\0';
                    char *path = start + 1;

                    char *content = (char*)(intptr_t)pop();

                    FILE *out = fopen(path, "wb");
                    if (out) {
                        fwrite(content, 1, strlen(content), out);
                        fclose(out);
                    }
                }
            }
        }

        /* ====== UNKNOWN ====== */
        else {
            fprintf(stderr, "Unknown OP: %s\n", cmd);
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr,
            "Usage: %s <op_file>\n",
            argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("open");
        return 1;
    }

    runtokens(f);
    fclose(f);
    return 0;
}
