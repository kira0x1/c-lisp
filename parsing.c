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

int main(int argc, char **argv) {
    // Parsers
    mpc_parser_t *Number = mpc_new("number");
    mpc_parser_t *Operator = mpc_new("operator");
    mpc_parser_t *Expr = mpc_new("expr");
    mpc_parser_t *Ab = mpc_new("ab");
    mpc_parser_t *Pengu = mpc_new("pengu");

    mpca_lang(MPCA_LANG_DEFAULT,
              "                                                         \
            number      :   /-?[0-9]+/ ;                                        \
            operator    :   '+' | '-' | '*' | '/' | /add/ | /mul/ | /sub/ | /div/ ;   \
            expr        :   <number> | '(' <operator> <expr>+ ')' ;             \
            ab          :    /[a-b]+/;                                          \
            pengu       :   /^/ <operator> <expr>+ | <ab> /$/ ;         \
            ",
              Number, Operator, Expr, Ab, Pengu);

    puts("pengu version 0.0.0.0.2");
    puts("press ctrl+c to exit\n");

    while (1) {
        char *input = readline("pengu> ");
        add_history(input);

        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Pengu, &r)) {
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
