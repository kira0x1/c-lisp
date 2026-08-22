#include "mpc.h"

int main(int argc, char **argv) {

  // Parsers
  mpc_parser_t *Number = mpc_new("number");
  mpc_parser_t *Operator = mpc_new("operator");
  mpc_parser_t *Expression = mpc_new("expr");
  mpc_parser_t *Pengu = mpc_new("pengu");

  mpca_lang(MPCA_LANG_DEFAULT,
            "number     : /-?[0-9]+/ ;                                      \
            operator    :   '+' | '-' | '*' | '/' ;                         \
            expr        :   <number> | '(' <operator> <expr>+ ')' ;         \
            pengu       :   /^/ <operator> <expr>_ /$/ ;                    \
            ",
            Number, Operator, Expression, Pengu);

  return 0;
}
