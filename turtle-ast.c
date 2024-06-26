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
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_VALUE;
  node->u.value = value;
  return node;
}

struct ast_node *make_expr_name(char *name) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_NAME;
  node->u.name = strcpy(calloc(strlen(name) + 1, sizeof(char)), name);;
  return node;
}

struct ast_node *make_expr_binop(char op, struct ast_node *expr1, struct ast_node *expr2) {
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
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_EXPR_UNOP;
  node->u.op = op;
  node->children_count = 1;
  node->children[0] = expr;
  node->next = NULL;
  return node;
}

struct ast_node *make_cmd_print(struct ast_node *expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_PRINT;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_cmd_up() {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_UP;
  return node;
}

struct ast_node *make_cmd_down() {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_DOWN;
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

struct ast_node *make_cmd_backward(struct ast_node *expr) {
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
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_RIGHT;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}

struct ast_node *make_cmd_left(struct ast_node *expr) {
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
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_COLOR;
  node->u.col = color;
  node->children_count = 0;
  return node;
}

struct ast_node *make_cmd_color(struct ast_node *expr) {
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
}

/*
 * context
 */

struct array *array_create(){
  struct array *array = calloc(1, sizeof(struct array));
  array->size = 0;
  array->capacity = 10;
  array->data = calloc(10, sizeof(struct ast_node*));
  return array;
}

void array_destroy(struct array *self){
  free(self->data);
  self->size = 0;
  self->capacity = 0;
  free(self);
}

void array_push_back(struct array *self, struct ast_node *node){
  if(self->size == self->capacity){
    self->capacity *= 2;
    self->data = realloc(self->data, self->capacity * sizeof(struct ast_node*));
  }
  for(size_t i = 0; i < self->size; ++i){
    if(strcmp(self->data[i]->children[0]->u.name, node->children[0]->u.name) == 0){
      self->data[i] = node;
      return;
    }
  }
  self->data[self->size++] = node;
}

struct ast_node *array_var_get(struct array *self, const char *name){
  for(size_t i = 0; i < self->size; ++i){
    if(strcmp(self->data[i]->children[0]->u.name, name) == 0){
      return self->data[i]->children[1];
    }
  }
  printf("Variable %s not found\n", name);
  exit(-1);
}

struct ast_node *array_proc_get(struct array *self, const char *name){
  for(size_t i = 0; i < self->size; ++i){
    if(strcmp(self->data[i]->children[0]->u.name, name) == 0){
      return self->data[i]->children[1];
    }
  }
  return NULL;
}


void context_create(struct context *self) {
  self->x = 0;
  self->y = 0;
  self->angle = 0;
  self->up = false;
  self->color = COLOR_BLACK;
  self->vars = array_create();
  self->procs = array_create();
  array_push_back(self->vars, make_cmd_set(make_expr_name("PI"), make_expr_value(PI)));
  array_push_back(self->vars, make_cmd_set(make_expr_name("SQRT2"), make_expr_value(SQRT2)));
  array_push_back(self->vars, make_cmd_set(make_expr_name("SQRT3"), make_expr_value(SQTR3)));
}

void context_destroy(struct context *self) {
  array_destroy(self->vars);
  array_destroy(self->procs);
  free(self);
}

/*
 * eval
 */

double deg_to_rad(double angle){
  return angle * PI / 180.0;
}

void ast_eval_node_cmd_simple(const struct ast_node *self, struct context *ctx){
  switch(self->u.cmd){
    case CMD_UP:
      ctx->up = true;
      break;
    case CMD_DOWN:
      ctx->up = false;
      break;
     case CMD_RIGHT:
      ctx->angle = ctx->angle + ast_eval_node(self->children[0], ctx);
      break;
    case CMD_LEFT:
      ctx->angle = ctx->angle - ast_eval_node(self->children[0], ctx);
      break;
    case CMD_HEADING:
      ctx->angle = ast_eval_node(self->children[0], ctx);
      break;
    case CMD_FORWARD:
      double val = ast_eval_node(self->children[0], ctx);
      ctx->y -= val * cos(deg_to_rad(ctx->angle));
      ctx->x += val * sin(deg_to_rad(ctx->angle));
      if(ctx->up){
        printf("MoveTo %f %f\n", ctx->x, ctx->y);
      }else{
        printf("LineTo %f %f\n", ctx->x, ctx->y);
      }
      break;
    case CMD_BACKWARD:
      val = ast_eval_node(self->children[0], ctx);
      ctx->y -= (-val) * cos(deg_to_rad(ctx->angle));
      ctx->x += (-val) * sin(deg_to_rad(ctx->angle));
      if(ctx->up){
        printf("MoveTo %f %f\n", ctx->x, ctx->y);
      }else{
        printf("LineTo %f %f\n", ctx->x, ctx->y);
      }
      break;
    case CMD_POSITION:
      ctx->x = ast_eval_node(self->children[0], ctx);
      ctx->y = ast_eval_node(self->children[1], ctx);
      if(ctx->up){
        printf("MoveTo %f %f\n", ctx->x, ctx->y);
      }else{
        printf("LineTo %f %f\n", ctx->x, ctx->y);
      }
      break;
    case CMD_HOME:
      ctx->x = 0;
      ctx->y = 0;
      ctx->angle = 0;
      ctx->up = false;
      ctx->color = COLOR_BLACK;
      printf("MoveTo 0.0 0.0\n");
      printf("Color 0.0 0.0 0.0\n");
      break;
    case CMD_COLOR:
      printf("Color ");
      if(self->children_count == 3){
        printf("%f %f %f\n", ast_eval_node(self->children[0], ctx), ast_eval_node(self->children[1], ctx), ast_eval_node(self->children[2], ctx));
      }else{
        ast_eval_node(self->children[0], ctx);
      }      
      break;
    case CMD_PRINT:
      printf("%f\n", ast_eval_node(self->children[0], ctx));
      break;    
  }
}

void ast_eval_node_cmd_repeat(const struct ast_node *self, struct context *ctx){
  int size = floor(ast_eval_node(self->children[0], ctx));
  for (int i = 0; i < size; i++) {
    ast_eval_node(self->children[1], ctx);
  }
}

void ast_eval_node_cmd_block(const struct ast_node *self, struct context *ctx){
}

void ast_eval_node_cmd_proc(const struct ast_node *self, struct context *ctx){
  struct ast_node *proc = array_proc_get(ctx->procs, self->children[0]->u.name);
  if(proc != NULL){
    printf("Redefinition of procedure %s\n", self->children[0]->u.name);
    exit(-1);
  }else{
    struct ast_node *node = self->children[1];
    while(node->next != NULL){
      if(node->kind == KIND_CMD_PROC){
        printf("Definition of procedure %s in procedure %s\n", node->children[0]->u.name, self->children[0]->u.name);
        exit(-1);
      }
      node = node->next;
    }
    array_push_back(ctx->procs, (struct ast_node*)self);
  }
}

void ast_eval_node_cmd_call(const struct ast_node *self, struct context *ctx){
  struct ast_node *proc = array_proc_get(ctx->procs, self->children[0]->u.name);
  if(proc == NULL){
    printf("Procedure %s not found\n", self->children[0]->u.name);
    exit(-1);
  }
  ast_eval_node(proc, ctx);
}

void ast_eval_node_cmd_set(const struct ast_node *self, struct context *ctx){
  array_push_back(ctx->vars, make_cmd_set(make_expr_name(self->children[0]->u.name), make_expr_value(ast_eval_node(self->children[1], ctx))));
}

double ast_eval_node_expr_func(const struct ast_node *self, struct context *ctx){
  switch(self->u.func){
    case FUNC_SIN:
      return sin(ast_eval_node(self->children[0], ctx));
      break;
    case FUNC_COS:
      return cos(ast_eval_node(self->children[0], ctx));
      break;
    case FUNC_TAN:
      return tan(ast_eval_node(self->children[0], ctx));
      break;
    case FUNC_SQRT:
      double value = ast_eval_node(self->children[0], ctx);
      if(value < 0){
        printf("Square root of a negative number\n");
        exit(-1);
      }
      return sqrt(value);
      break;
    case FUNC_RANDOM:
      double lower_bound = ast_eval_node(self->children[0], ctx);
      double upper_bound = ast_eval_node(self->children[1], ctx);
      if(lower_bound > upper_bound){
        printf("Lower bound greater than upper bound\n");
        exit(-1);
      }
      return ((((float) rand()) / (float) RAND_MAX) * (upper_bound - lower_bound) + lower_bound);
      break;
    default:
      printf("Unknown function\n");
      exit(-1);
      break;
  }
}

void ast_eval_node_expr_value(const struct ast_node *self, struct context *ctx){}

double ast_eval_node_expr_unop(const struct ast_node *self, struct context *ctx){
  switch(self->u.op){
    case '-':
      return -ast_eval_node(self->children[0], ctx);
      break;
    case '+':
      return ast_eval_node(self->children[0], ctx);
      break;
    default:
      printf("Unknown operator\n");
      exit(-1);
      break;
  }
}

double ast_eval_node_expr_binop(const struct ast_node *self, struct context *ctx){
  switch(self->u.op){
    case '+':
      return ast_eval_node(self->children[0], ctx) + ast_eval_node(self->children[1], ctx);
      break;
    case '-' :
      return ast_eval_node(self->children[0], ctx) - ast_eval_node(self->children[1], ctx);
      break;
    case '*' :
      return ast_eval_node(self->children[0], ctx) * ast_eval_node(self->children[1], ctx);
      break;
    case '/' :
      if(ast_eval_node(self->children[1], ctx) == 0){
        printf("Division by zero\n");
        exit(-1);
      }
      return ast_eval_node(self->children[0], ctx) / ast_eval_node(self->children[1], ctx);
      break;
    case '^' :
      double value = ast_eval_node(self->children[0], ctx);
      double power = ast_eval_node(self->children[1], ctx);
      if(value < 0 && power != floor(power)){
        printf("Negative number raised to a non-integer power\n");
        exit(-1);
      }
      if(value == 0 && power < 0){
        printf("Zero raised to a negative power\n");
        exit(-1);
      }
      return pow(value, power);
      break;
    default:
      printf("Unknown operator\n");
      exit(-1);
      break;
  }
}

void ast_eval_node_expr_block(const struct ast_node *self, struct context *ctx){}

double ast_eval_node_expr_name(const struct ast_node *self, struct context *ctx){
  struct ast_node *value = array_var_get(ctx->vars, self->u.name);
  return value->u.value;
}


void ast_eval_node_cmd_color(const struct ast_node *self, struct context *ctx){
  if(self->children_count == 0){
    switch(self->u.col){
      case COLOR_BLACK:
        printf("0.0 0.0 0.0\n");
        break;
      case COLOR_WHITE:
        printf("1.0 1.0 1.0\n");
        break;
      case COLOR_RED:
        printf("1.0 0.0 0.0\n");
        break;
      case COLOR_GREEN:
        printf("0.0 1.0 0.0\n");
        break;
      case COLOR_BLUE:
        printf("0.0 0.0 1.0\n");
        break;
      case COLOR_YELLOW:
        printf("1.0 1.0 0.0\n");
        break;
      case COLOR_CYAN:
        printf("0.0 1.0 1.0\n");
        break;
      case COLOR_MAGENTA:
        printf("1.0 0.0 1.0\n");
        break;
      case COLOR_GREY:
        printf("0.5 0.5 0.5\n");
        break;
      default:
        printf("Unknown color\n");
        exit(-1);
    }
  }
}

double ast_eval_node(const struct ast_node *self, struct context *ctx){
  if(self == NULL){
    return NAN;
  }
  double result = 0;
  switch(self->kind){
    case KIND_CMD_SIMPLE:
      ast_eval_node_cmd_simple(self, ctx);
      break;
    case KIND_CMD_REPEAT:
      ast_eval_node_cmd_repeat(self, ctx);
      break;
    case KIND_CMD_BLOCK:
      ast_eval_node_cmd_block(self, ctx);
      break;
    case KIND_CMD_PROC:
      ast_eval_node_cmd_proc(self, ctx);
      break;
    case KIND_CMD_CALL:
      ast_eval_node_cmd_call(self, ctx);
      break;
    case KIND_CMD_SET:
      ast_eval_node_cmd_set(self, ctx);
      break;
    case KIND_EXPR_FUNC:
      result = ast_eval_node_expr_func(self, ctx);
      break;
    case KIND_EXPR_VALUE:
      result = self->u.value;
      break;
    case KIND_EXPR_UNOP:  
      result = ast_eval_node_expr_unop(self, ctx);
      break;
    case KIND_EXPR_BINOP:
      result = ast_eval_node_expr_binop(self, ctx);
      break;
    case KIND_EXPR_BLOCK:
      result = ast_eval_node(self->children[0], ctx);
      break;
    case KIND_EXPR_NAME:
      result = ast_eval_node_expr_name(self, ctx);
      break;
    case KIND_CMD_COLOR:
      ast_eval_node_cmd_color(self, ctx);
      break;
  }
  ast_eval_node(self->next, ctx);
  return result;
}

void ast_eval(const struct ast *self, struct context *ctx) {
  ast_eval_node(self->unit, ctx);
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
      break;
    case CMD_LEFT:
      printf("left ");
      ast_node_print(self->children[0]);
      printf("\n");
      break;
    case CMD_HEADING:
      printf("heading ");
      ast_node_print(self->children[0]);
      printf("\n");
      break;
    case CMD_FORWARD:
      printf("fw ");
      ast_node_print(self->children[0]);
      printf("\n");
      break;
    case CMD_BACKWARD:
      printf("bw ");
      ast_node_print(self->children[0]);
      printf("\n");
      break;
    case CMD_POSITION:
      printf("pos ");
      ast_node_print(self->children[0]);
      printf(" ");
      ast_node_print(self->children[1]);
      printf("\n");
      break;
    case CMD_HOME:
      printf("home");
      printf("\n");
      break;
    case CMD_COLOR:
      printf("color ");
      if(self->children_count == 1){
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
  printf("\n");
}

void ast_node_print_cmd_set(const struct ast_node *self){
  printf("set ");
  ast_node_print_expr_name(self->children[0]);
  printf(" ");
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
  ast_node_print(self->next);
}

void ast_print(const struct ast *self) {
  ast_node_print(self->unit);
  printf("\n");
}
