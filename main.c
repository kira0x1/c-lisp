#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <string.h>

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

// buffer for user input
// static char input[2048];

int main(int argc, char **argv) {
    puts("pengu version 0.0.0.0.1");
    puts("press ctrl+c to exit\n");

    while (1) {
        char *input = readline("pengu> ");

        add_history(input);

        // fgets(input, 2048, stdin);

        int len = strlen(input);
        if (len > 0) {
            printf("No you're a %s\n", input);
        }

        free(input);
    }

    return 0;
}
