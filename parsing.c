#include "mpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

static char buffer[2048];

/* Fake readline function */
char *readline(char *prompt) {
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);
    char *cpy = malloc(strlen(buffer) + 1);
    strcpy_s(cpy, 2048, buffer);
    cpy[strlen(cpy) - 1] = '\0';
    return cpy;
}

/* Fake add_history function */
void add_history(char *unused) {
}

/* Otherwise include the editline headers */
#else
#include <editline/history.h>
#include <editline/readline.h>
#endif

int number_of_nodes(mpc_ast_t *t) {
    if (t->children_num == 0) { return 1; }
    if (t->children_num >= 1) {
        int total = 1;
        for (int i = 0; i < t->children_num; i++) {
            total += number_of_nodes(t->children[i]);
        }
        return total;
    }
    return 0;
}


int main(int argc, char **argv) {
    // Parsers
    mpc_parser_t *Number = mpc_new("number");
    mpc_parser_t *Operator = mpc_new("operator");
    mpc_parser_t *Expr = mpc_new("expr");
    mpc_parser_t *Ab = mpc_new("ab");
    mpc_parser_t *Pengu = mpc_new("pengu");

    mpca_lang(MPCA_LANG_DEFAULT,
              "                                                        \
            number      :   /-?[0-9]+/ ;                                        \
            operator    :   '+' | '-' | '*' | '/' | /add/ | /mul/ | /sub/ | /div/ ;   \
            expr        :   <number> | '(' <operator> <expr>+ ')' ;             \
            ab          :    /[a-b]+/;                                          \
            pengu       :   /^/ <operator> <expr>+ | <ab> /$/ ;                 \
            ",
              Number, Operator, Expr, Ab, Pengu);

    puts("pengu version 0.0.0.0.2");
    puts("press ctrl+c to exit\n");

    while (1) {
        char *input = readline("pengu> ");
        add_history(input);

        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Pengu, &r)) {
            mpc_ast_t *a = r.output;
            printf("\ntag: %s\n", a->tag);
            printf("contents: %s\n", a->contents);
            printf("children: %i\n\n", a->children_num);


            mpc_ast_t *c0 = a->children[0];
            printf("child-0 [TAG]: %s\n", c0->tag);
            printf("child-0 [CONTENTS]: %s\n", c0->contents);
            printf("child-0 [NUM-CHILDREN]: %i\n\n", c0->children_num);

            printf("---------------- number of nodes ------------\n%i\n---------------------\n", number_of_nodes(a));

            mpc_ast_print(r.output);
            mpc_ast_delete(r.output);
        } else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        free(input);
    }

    mpc_cleanup(5, Number, Operator, Expr, Ab, Pengu);

    return 0;
}
