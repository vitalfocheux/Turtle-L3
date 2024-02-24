#include "turtle-ast.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.141592653589793
#define SQRT2 1.41421356237309504880
#define SQTR3 1.7320508075688772935

struct ast_node *make_expr_value(double value) {
  // printf("make_expr_value\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_VALUE;
  node->u.value = value;
  return node;
}

struct ast_node *make_expr_name(char *name) {
  // printf("make_expr_name\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_NAME;
  node->u.name = strcpy(calloc(strlen(name) + 1, sizeof(char)), name);;
  return node;
}

struct ast_node *make_expr_binop(char op, struct ast_node *expr1, struct ast_node *expr2) {
  // printf("make_expr_binop\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_BINOP;
  node->u.op = op;
  node->children_count = 2;
  node->children[0] = expr1;
  node->children[1] = expr2;
  node->next = NULL;
  return node;
}

struct ast_node *make_expr_unop(char op, struct ast_node *expr){
  // printf("make_expr_unop\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_UNOP;
  node->u.op = op;
  node->children_count = 1;
  node->children[0] = expr;
  node->next = NULL;
  return node;
}

struct ast_node *make_cmd_print(struct ast_node *expr) {
  // printf("make_cmd_print\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_PRINT;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_cmd_up() {
  // printf("make_cmd_up\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_UP;
  return node;
}

struct ast_node *make_cmd_down() {
  // printf("make_cmd_down\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_DOWN;
  return node;
}

struct ast_node *make_cmd_forward(struct ast_node *expr) {
  // printf("make_cmd_forward\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_FORWARD;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_cmd_backward(struct ast_node *expr) {
  // printf("make_cmd_backward\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_BACKWARD;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_cmd_pos(struct ast_node *expr1, struct ast_node *expr2) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_POSITION;
  node->children_count = 2;
  node->children[0] = expr1;
  node->children[1] = expr2;
  return node;
}

struct ast_node *make_cmd_right(struct ast_node *expr) {
  // printf("make_cmd_right\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_RIGHT;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_cmd_left(struct ast_node *expr) {
  // printf("make_cmd_left\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_LEFT;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_cmd_heading(struct ast_node *expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_HEADING;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_cmd_color_RGB(struct ast_node *red, struct ast_node *green, struct ast_node *blue) {
  // printf("make_cmd_color_RGB\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_COLOR;
  node->children_count = 3;
  node->children[0] = red;
  node->children[1] = green;
  node->children[2] = blue;
  return node;
}

struct ast_node *make_cmd_color_name(enum color color){

  // struct ast_node *red = calloc(1, sizeof(struct ast_node));
  // red->kind = KIND_EXPR_VALUE;

  // struct ast_node *green = calloc(1, sizeof(struct ast_node));
  // green->kind = KIND_EXPR_VALUE;

  // struct ast_node *blue = calloc(1, sizeof(struct ast_node));
  // blue->kind = KIND_EXPR_VALUE;

  // switch(color){
  //   case COLOR_BLACK:
  //     red->u.value = 0;
  //     green->u.value = 0;
  //     blue->u.value = 0;
  //     return make_cmd_color_RGB(red, green, blue);
  //   case COLOR_WHITE:
  //     red->u.value = 1;
  //     green->u.value = 1;
  //     blue->u.value = 1;
  //     return make_cmd_color_RGB(red, green, blue);
  //   case COLOR_RED:
  //     red->u.value = 1;
  //     green->u.value = 0;
  //     blue->u.value = 0;
  //     return make_cmd_color_RGB(red, green, blue);
  //   case COLOR_GREEN:
  //     red->u.value = 0;
  //     green->u.value = 1;
  //     blue->u.value = 0;
  //     return make_cmd_color_RGB(red, green, blue);
  //   case COLOR_BLUE:
  //     red->u.value = 0;
  //     green->u.value = 0;
  //     blue->u.value = 1;
  //     return make_cmd_color_RGB(red, green, blue);
  //   case COLOR_YELLOW:
  //     red->u.value = 1;
  //     green->u.value = 1;
  //     blue->u.value = 0;
  //     return make_cmd_color_RGB(red, green, blue);
  //   case COLOR_CYAN:
  //     red->u.value = 0;
  //     green->u.value = 1;
  //     blue->u.value = 1;
  //     return make_cmd_color_RGB(red, green, blue);
  //   case COLOR_MAGENTA:
  //     red->u.value = 1;
  //     green->u.value = 0;
  //     blue->u.value = 1;
  //     return make_cmd_color_RGB(red, green, blue);
  //   case COLOR_GREY:
  //     red->u.value = 0.5;
  //     green->u.value = 0.5;
  //     blue->u.value = 0.5;
  //     return make_cmd_color_RGB(red, green, blue);
  //   default:
  //     printf("Unknown color\n");
  //     exit(-1);
  // }

  // return NULL;
  // printf("make_cmd_color_name\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_COLOR;
  node->u.col = color;
  node->children_count = 0;
  return node;
}

struct ast_node *make_cmd_color(struct ast_node *expr) {
  // printf("make_cmd_color\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_COLOR;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_cmd_home(){
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_HOME;
  return node;
}

struct ast_node *make_cmd_repeat(struct ast_node *expr, struct ast_node *block) {
  // printf("make_cmd_repeat\n");
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_REPEAT;
  node->children_count = 2;
  node->children[0] = expr;
  node->children[1] = block;
  return node;
}

struct ast_node *make_cmd_set(struct ast_node *name, struct ast_node *expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SET;
  node->children_count = 2;
  node->children[0] = name;
  node->children[1] = expr;
  return node;
}

struct ast_node *make_cmd_proc(struct ast_node *name, struct ast_node *block) {
  // printf("make_cmd_proc\n");
  // ast_node_print(name);
  // printf("%d\n", name->kind);
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_PROC;
  node->children_count = 2;
  node->children[0] = name;
  node->children[1] = block;
  return node;
}

struct ast_node *make_cmd_call(struct ast_node *name) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_CALL;
  node->children_count = 1;
  node->children[0] = name;
  return node;
}

struct ast_node *make_cmd_block(struct ast_node *block) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_BLOCK;
  node->children_count = 1;
  node->children[0] = block;
  return node;
}

struct ast_node *make_math_sin(struct ast_node *expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_FUNC;
  node->u.func = FUNC_SIN;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_math_cos(struct ast_node *expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_FUNC;
  node->u.func = FUNC_COS;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_math_tan(struct ast_node *expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_FUNC;
  node->u.func = FUNC_TAN;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_math_sqrt(struct ast_node *expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_FUNC;
  node->u.func = FUNC_SQRT;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_math_random(struct ast_node *lower_bound, struct ast_node *upper_bound) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_FUNC;
  node->u.func = FUNC_RANDOM;
  node->children_count = 2;
  node->children[0] = lower_bound;
  node->children[1] = upper_bound;
  return node;
}

struct ast_node *make_expr_block(struct ast_node *block) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_BLOCK;
  node->children_count = 1;
  node->children[0] = block;
  return node;
}

void ast_node_destroy(struct ast_node *self) {
  for(size_t i = 0; i < self->children_count; ++i){
    ast_node_destroy(self->children[i]);
  }

  if(self->next == NULL){
    free(self);
  }else{
    ast_node_destroy(self->next);
    self->next = NULL;
    free(self);
  }
}

void ast_destroy(struct ast *self) {
  ast_node_destroy(self->unit);
  // free(self);
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

void ast_node_print_cmd_simple(const struct ast_node *self){
  switch(self->u.cmd){
    case CMD_UP:
      printf("up\n");
      break;
    case CMD_DOWN:
      printf("down\n");
      break;
    case CMD_RIGHT:
      printf("right ");
      ast_node_print(self->children[0]);
      printf("\n");
      // ast_node_print_expr_value(self->children[0]);
      break;
    case CMD_LEFT:
      printf("left ");
      ast_node_print(self->children[0]);
      printf("\n");
      // ast_node_print_expr_value(self->children[0]);
      break;
    case CMD_HEADING:
      printf("heading ");
      ast_node_print(self->children[0]);
      printf("\n");
      // ast_node_print_expr_value(self->children[0]);
      break;
    case CMD_FORWARD:
      printf("fw ");
      // printf("%d ", self->children[0]->kind);
      ast_node_print(self->children[0]);
      printf("\n");
      // ast_node_print_expr_value(self->children[0]);
      break;
    case CMD_BACKWARD:
      printf("bw ");
      ast_node_print(self->children[0]);
      printf("\n");
      // ast_node_print_expr_value(self->children[0]);
      break;
    case CMD_POSITION:
      printf("pos ");
      ast_node_print(self->children[0]);
      // ast_node_print_expr_value(self->children[0]);
      printf(" ");
      ast_node_print(self->children[1]);
      printf("\n");
      // ast_node_print_expr_value(self->children[0]);
      break;
    case CMD_HOME:
      printf("home");
      printf("\n");
      break;
    case CMD_COLOR:
      printf("color ");
      // printf("%ld ", self->children_count);
      if(self->children_count == 1){
        // ast_node_print(self->children[0]);
        switch(self->children[0]->u.col){
          case COLOR_BLACK:
            printf("black");
            break;
          case COLOR_WHITE:
            printf("white");
            break;
          case COLOR_RED:
            printf("red");
            break;
          case COLOR_GREEN:
            printf("green");
            break;
          case COLOR_BLUE:
            printf("blue");
            break;
          case COLOR_YELLOW:
            printf("yellow");
            break;
          case COLOR_CYAN:
            printf("cyan");
            break;
          case COLOR_MAGENTA:
            printf("magenta");
            break;
          case COLOR_GREY:
            printf("grey");
            break;
          default:
            printf("unknown color");
            break;
        }
      }else if(self->children_count == 3){
        ast_node_print(self->children[0]);
        printf(", ");
        ast_node_print(self->children[1]);
        printf(", ");
        ast_node_print(self->children[2]);
      }
      printf("\n");
      break;
    case CMD_PRINT:
      printf("print ");
      ast_node_print(self->children[0]);
      // ast_node_print_expr_name(self->children[0]);
      printf("\n");
      break;
  }
}

void ast_node_print_cmd_repeat(const struct ast_node *self){
  printf("repeat ");
  ast_node_print(self->children[0]);
  printf(" {\n");
  ast_node_print(self->children[1]);
  printf("}\n");
}

void ast_node_print_cmd_block(const struct ast_node *self){
  printf("{");
  ast_node_print(self->children[0]);
  printf("}");
}

void ast_node_print_cmd_proc(const struct ast_node *self){
  printf("proc ");
  ast_node_print(self->children[0]);
  printf(" {\n");
  ast_node_print(self->children[1]);
  printf("}\n");
}

void ast_node_print_cmd_call(const struct ast_node *self){
  printf("call ");
  ast_node_print_expr_name(self->children[0]);
}

void ast_node_print_cmd_set(const struct ast_node *self){
  printf("set ");
  ast_node_print_expr_name(self->children[0]);
  printf(" ");
  ast_node_print(self->children[1]);
}

void ast_node_print_expr_func(const struct ast_node *self){
  switch(self->u.func){
    case FUNC_SIN:
      printf("sin (");
      ast_node_print(self->children[0]);
      printf(")");
      break;
    case FUNC_COS:
      printf("cos (");
      ast_node_print(self->children[0]);
      printf(")");
      break;
    case FUNC_TAN:
      printf("tan (");
      ast_node_print(self->children[0]);
      printf(")");
      break;
    case FUNC_SQRT:
      printf("sqrt (");
      ast_node_print(self->children[0]);
      printf(")");
      break;
    case FUNC_RANDOM:
      printf("random(");
      ast_node_print(self->children[0]);
      printf(", ");
      ast_node_print(self->children[1]);
      printf(")");
      break;
  
  }
}

void ast_node_print_expr_value(const struct ast_node *self){
  printf("%f ", self->u.value);
}

void ast_node_print_expr_unop(const struct ast_node *self){
  printf("%c ", self->u.op);
  ast_node_print(self->children[0]);
}

void ast_node_print_expr_binop(const struct ast_node *self){
  ast_node_print(self->children[0]);
  printf("%c ", self->u.op);
  ast_node_print(self->children[1]);
}

void ast_node_print_expr_block(const struct ast_node *self){
  printf("(");
  ast_node_print(self->children[0]);
  printf(")");
}

void ast_node_print_expr_name(const struct ast_node *self){
  printf("%s ", self->u.name);
}

void ast_node_print(const struct ast_node *self) {
  if(self == NULL) {
    // printf("NULL");
    return;
  }
  switch (self->kind)
  {
  case KIND_CMD_SIMPLE:
    ast_node_print_cmd_simple(self);
    break;
  case KIND_CMD_REPEAT:
    ast_node_print_cmd_repeat(self);
    break;
  case KIND_CMD_BLOCK:
    ast_node_print_cmd_block(self);
    break;
  case KIND_CMD_PROC:
    ast_node_print_cmd_proc(self);
    break;
  case KIND_CMD_CALL:
    ast_node_print_cmd_call(self);
    break;
  case KIND_CMD_SET:
    ast_node_print_cmd_set(self);
    break;  
  case KIND_EXPR_FUNC:
    ast_node_print_expr_func(self);
    break;
  case KIND_EXPR_VALUE:
    ast_node_print_expr_value(self);
    break;
  case KIND_EXPR_UNOP:
    ast_node_print_expr_unop(self);
    break;
  case KIND_EXPR_BINOP:
    ast_node_print_expr_binop(self);
    break;
  case KIND_EXPR_BLOCK:
    ast_node_print_expr_block(self);
    break;
  case KIND_EXPR_NAME:
    ast_node_print_expr_name(self);
    break;
  default:
    printf("default");
    break;
  }
  // for(size_t i = 0; i < self->children_count; ++i){
  //   ast_node_print(self->children[i]);
  // }
  // if(self->next == NULL){
  //   printf("\n");
  // }
  // printf("\n");
  ast_node_print(self->next);
}

void ast_print(const struct ast *self) {
  ast_node_print(self->unit);
  printf("\n");
}
