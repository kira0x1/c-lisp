#include <stdio.h>

// buffer for user input
static char input[2048];

int main(int argc, char **argv) {

  puts("pengu version 0.0.0.0.1");
  puts("press ctrl+c to exit\n");

  while (1) {
    fputs("pengu> ", stdout);

    fgets(input, 2048, stdin);

    printf("No you're a %s", input);
  }

  return 0;
}
