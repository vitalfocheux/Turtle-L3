#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "turtle-ast.h"
#include "turtle-lexer.h"
#include "turtle-parser.h"

#define EVAL

int main() {
  srand(time(NULL));
  
  struct ast root;
  int ret = yyparse(&root);

  if (ret != 0) {
    printf("Error: %d\n", ret);
    return ret;
  }
  
  yylex_destroy();

  assert(root.unit);

  #ifdef EVAL

  struct context ctx;
  context_create(&ctx);
  
  ast_eval(&root, &ctx);
  // context_destroy(&ctx);

  #else 
  
  ast_print(&root);

  #endif

  ast_destroy(&root);

  return ret;
}
