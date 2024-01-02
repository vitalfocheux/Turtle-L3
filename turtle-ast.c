#include "turtle-ast.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.141592653589793

struct ast_node *make_expr_value(double value) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_VALUE;
  node->u.value = value;
  return node;
}

struct ast_node *make_cmd_forward(struct ast_node *expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_FORWARD;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

void ast_node_destroy(struct ast_node *self) {
  if (self == NULL) {
    return;
  }
  for (size_t i = 0; i < self->children_count; i++) {
    ast_node_destroy(self->children[i]);
  }
  ast_node_destroy(self->next);
  free(self);
}

void ast_destroy(struct ast *self) {
  ast_node_destroy(self->unit);
  free(self);
}

/*
 * context
 */

void context_create(struct context *self) {

}

/*
 * eval
 */

void ast_eval(const struct ast *self, struct context *ctx) {

}

/*
 * print
 */

void ast_node_print(const struct ast_node *self) {
  if (self == NULL) {
    return;
  }
  switch (self->kind) {
    case KIND_CMD_SIMPLE:
      printf("cmd_simple");
      break;
    case KIND_EXPR_VALUE:
      printf("%g", self->u.value);
      break;
    case KIND_EXPR_BINOP:
      printf("(");
      ast_node_print(self->children[0]);
      printf(" %c ", self->u.op);
      ast_node_print(self->children[1]);
      printf(")");
      break;
    case KIND_EXPR_UNOP:
      printf("(");
      printf("%c", self->u.op);
      ast_node_print(self->children[0]);
      printf(")");
      break;
    case KIND_EXPR_NAME:
      printf("%s", self->u.name);
      break;
    case KIND_EXPR_FUNC:
      printf("function");
      for (size_t i = 0; i < self->children_count; i++) {
        ast_node_print(self->children[i]);
        if (i + 1 < self->children_count) {
          printf(", ");
        }
      }
      printf(")");
      break;
    default:
      printf("unknown");
      break;
  }
  printf("\n");
  ast_node_print(self->next);  
}

void ast_print(const struct ast *self) {
  ast_node_print(self->unit);
  printf("\n");
}
