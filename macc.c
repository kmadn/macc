#include <stdio.h>
#include <ctype.h>
#include<string.h>
#include <stdlib.h>

/* token's type */
enum {
    TK_NUM = 256, // Integer token
    TK_EOF,       // EOF token
};

typedef struct {
    int ty;       // token's type
    int val;      // token's value (integer)
    char* input;  // token string (error case)
}Token;

/* Buffer */
Token tokens[100];

/* String splits token, and stores buffer */
void tokenize (char* p) {
    int i = 0;
    while(*p) {
        /* Skip blank */
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (*p == '+' || *p == '-') {
            tokens[i].ty = *p;
            tokens[i].input = p;
            i++;
            p++;
            continue;
        }

            if (isdigit(*p)) {
            tokens[i].ty = TK_NUM;
            tokens[i].input = p;
            tokens[i].val = strtol(p, &p, 10);
            i++;
            continue;
        }

        fprintf(stderr, "don't tokenize: %s\n", p);
        exit(1);
    }

    tokens[i].ty = TK_EOF;
    tokens[i].input = p;
}

/* Notify error */
void error (int i) {
    fprintf(stderr, "Unexpected token: %s\n", tokens[i].input);
    exit(1);
}

int main(int argc, char** argv){
    if(argc != 2){
        fprintf(stderr,"Arguments error\n");
        return 1;
    }

    tokenize(argv[1]);

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    /* First token must be integer */
    if (tokens[0].ty != TK_NUM)
        error(0);
    printf("  mov rax, %d\n", tokens[0].val);

    int i = 1;
    while(tokens[i].ty != TK_EOF) {
        if (tokens[i].ty == '+'){
            i++;
            if(tokens[i].ty != TK_NUM)
                error(i);
            printf("  add rax, %d\n", tokens[i].val);
            i++;
            continue;
        }

        if (tokens[i].ty == '-') {
            i++;
            if (tokens[i].ty != TK_NUM) 
                error(i);
            printf("  sub rax, %d\n", tokens[i].val);
            i++;
            continue;
        }
 
        error(i);
    }
    
    printf("  ret\n");
    return 0;
}
