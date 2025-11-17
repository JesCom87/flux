// flux_vm.c  -- compile with: gcc -o flux_vm flux_vm.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 1024
long stack[STACK_SIZE];
int sp = 0;

void push(long v){ if(sp<STACK_SIZE) stack[sp++]=v; }
long pop(){ if(sp>0) return stack[--sp]; return 0; }

void runtokens(FILE *f){
    char line[1024];
    while(fgets(line,sizeof(line),f)){
        // trim newline
        char *p = line;
        while(*p && (*p==' '||*p=='\t')) p++;
        if(*p=='#' || *p=='\0') continue;

        char cmd[64];
        if(sscanf(p, "%63s", cmd)!=1) continue;

        if(strcmp(cmd,"CONST")==0){
            // CONST <number> or CONST "string"
            p += strlen("CONST");
            while(*p==' '||*p=='\t') p++;
            if(*p=='"'){
                // string literal -> push pointer via printing at PRINT time
                // We'll store string index as negative numbers: pointer stored separately
                // For simplicity, print stored string immediately and push 0 marker if needed.
                // Here we store string in a simple heap: print on PRINT.
                char *start = strchr(p,'"');
                char *end = start? strchr(start+1,'"'):NULL;
                if(start && end){
                    *end = '\0';
                    // print immediate by storing a negative index? Simpler: push special marker and store string into a file-backed list.
                    // For this simple VM: print string immediately to stdout only when PRINT consumes.
                    // Here we'll push ASCII codes? keep it simple: push the address to a static buffer (dangerous but demo).
                    char *s = strdup(start+1);
                    // push pointer casted to long (works on typical 64-bit, but be mindful on 32-bit)
                    push((long)(intptr_t)s);
                }
            } else {
                long n = atol(p);
                push(n);
            }
        } else if(strcmp(cmd,"PRINT")==0){
            // if top of stack is pointer/str -> print string; else print number
            long v = pop();
            if(v==0){
                printf("0\n");
            } else {
                char *s = (char*)(intptr_t)v;
                // Heuristic: if looks like pointer (non-digit) print as string
                // If string was stored, we print
                if(s && (s[0]<'0' || s[0]>'9')){
                    printf("%s\n", s);
                } else {
                    printf("%ld\n", (long)v);
                }
            }
        } else if(strcmp(cmd,"ADD")==0){
            long b = pop(); long a = pop(); push(a+b);
        } else if(strcmp(cmd,"MUL")==0){
            long b = pop(); long a = pop(); push(a*b);
        } else {
            // Unknown op
            fprintf(stderr,"unknown op: %s\n", cmd);
        }
    }
}

int main(int argc, char **argv){
    if(argc < 2){
        fprintf(stderr,"Usage: %s <op_file>\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1],"r");
    if(!f){ perror("open"); return 1; }
    runtokens(f);
    fclose(f);
    return 0;
}
