#ifndef TURTLE_AST_H
#define TURTLE_AST_H

#include <stddef.h>
#include <stdbool.h>

// simple commands
enum ast_cmd {
  CMD_UP,
  CMD_DOWN,
  CMD_RIGHT,
  CMD_LEFT,
  CMD_HEADING,
  CMD_FORWARD,
  CMD_BACKWARD,
  CMD_POSITION,
  CMD_HOME,
  CMD_COLOR,
  CMD_PRINT,
};

// internal functions
enum ast_func {
  FUNC_COS,
  FUNC_RANDOM,
  FUNC_SIN,
  FUNC_SQRT,
  FUNC_TAN,
};

// kind of a node in the abstract syntax tree
enum ast_kind {
  KIND_CMD_SIMPLE,
  KIND_CMD_REPEAT,
  KIND_CMD_BLOCK,
  KIND_CMD_PROC,
  KIND_CMD_CALL,
  KIND_CMD_SET,

  KIND_EXPR_FUNC,
  KIND_EXPR_VALUE,
  KIND_EXPR_UNOP,
  KIND_EXPR_BINOP,
  KIND_EXPR_BLOCK,
  KIND_EXPR_NAME,

  KIND_CMD_COLOR,
};

/**
 * Color of a node in the abstract syntax tree 
*/
enum color{
  COLOR_RED,
  COLOR_GREEN,
  COLOR_BLUE,
  COLOR_CYAN,
  COLOR_MAGENTA,
  COLOR_YELLOW,
  COLOR_BLACK,
  COLOR_GREY,
  COLOR_WHITE,
};

#define AST_CHILDREN_MAX 3

// a node in the abstract syntax tree
struct ast_node {
  enum ast_kind kind; // kind of the node

  union {
    enum ast_cmd cmd;   // kind == KIND_CMD_SIMPLE
    double value;       // kind == KIND_EXPR_VALUE, for literals
    char op;            // kind == KIND_EXPR_BINOP or kind == KIND_EXPR_UNOP, for operators in expressions
    char *name;         // kind == KIND_EXPR_NAME, the name of procedures and variables
    enum ast_func func; // kind == KIND_EXPR_FUNC, a function
    enum color col;     // kind == KIND_CMD_COLOR, a color
  } u;

  size_t children_count;  // the number of children of the node
  struct ast_node *children[AST_CHILDREN_MAX];  // the children of the node (arguments of commands, etc)
  struct ast_node *next;  // the next node in the sequence
};




// TODO: make some constructors to use in parser.y
// for example:

/**
 * Create a new node in the abstract syntax tree for functions 
*/
struct ast_node *make_expr_func(enum ast_func func, struct ast_node *expr);

/**
 * Create a new node in the abstract syntax tree for numeric values
*/
struct ast_node *make_expr_value(double value);

/**
 * Create a new node in the abstract syntax tree for unary operations
*/
struct ast_node *make_expr_unop(char op, struct ast_node *expr);

/**
 * Create a new node in the abstract syntax tree for binary operations
*/
struct ast_node *make_expr_binop(char op, struct ast_node *expr1, struct ast_node *expr2);

/**
 * Create a new node in the abstract syntax tree for expression blocks
*/
struct ast_node *make_expr_block(struct ast_node *block);

/**
 * Create a new node in the abstract syntax tree for expressions of variable and procedure names
*/
struct ast_node *make_expr_name(char *name);

/**
 * Create a new node in the abstract syntax tree for the 'print' command
*/
struct ast_node *make_cmd_print(struct ast_node *expr);

/**
 * Create a new node in the abstract syntax tree for the 'up' command
*/
struct ast_node *make_cmd_up();

/**
 * Create a new node in the abstract syntax tree for the 'down' command
*/
struct ast_node *make_cmd_down();

/**
 * Create a new node in the abstract syntax tree for the 'forward' command
*/
struct ast_node *make_cmd_forward(struct ast_node *expr);

/**
 * Create a new node in the abstract syntax tree for the 'backward' command
*/
struct ast_node *make_cmd_backward(struct ast_node *expr);

/**
 * Create a new node in the abstract syntax tree for the 'pos' command
*/
struct ast_node *make_cmd_pos(struct ast_node *expr1, struct ast_node *expr2);

/**
 * Create a new node in the abstract syntax tree for the 'right' command
*/
struct ast_node *make_cmd_right(struct ast_node *expr);

/**
 * Create a new node in the abstract syntax tree for the 'left' command
*/
struct ast_node *make_cmd_left(struct ast_node *expr);

/**
 * Create a new node in the abstract syntax tree for the 'heading' command
*/
struct ast_node *make_cmd_heading(struct ast_node *expr);

/**
 * Create a new node in the abstract syntax tree for the 'color' command 
 * with three color values ranging from 0 to 1 as parameters
*/
struct ast_node *make_cmd_color_RGB(struct ast_node *red, struct ast_node *green, struct ast_node *blue);

/**
 * Create a new node in the abstract syntax tree for the color command 
 * with the name of a color as a parameter
*/
struct ast_node *make_cmd_color_name(enum color color);

/**
 * Create a new node in the abstract syntax tree for the 'color' command
*/
struct ast_node *make_cmd_color(struct ast_node *expr);

/**
 * Create a new node in the abstract syntax tree for the 'home' command
*/
struct ast_node *make_cmd_home();

/**
 * Create a new node in the abstract syntax tree for the 'repeat' command
*/
struct ast_node *make_cmd_repeat(struct ast_node *expr, struct ast_node *block);

/**
 * Create a new node in the abstract syntax tree for the 'set' command
*/
struct ast_node *make_cmd_set(struct ast_node *name, struct ast_node *expr);

/**
 * Create a new node in the abstract syntax tree for the 'proc' command
*/
struct ast_node *make_cmd_proc(struct ast_node *name, struct ast_node *block);

/**
 * Create a new node in the abstract syntax tree for the 'call' command
*/
struct ast_node *make_cmd_call(struct ast_node *name);

/**
 * Create a new node in the abstract syntax tree for command blocks
*/
struct ast_node *make_cmd_block(struct ast_node *block);

/**
 * Create a new node in the abstract syntax tree for the sine function
*/
struct ast_node *make_math_sin(struct ast_node *expr);

/**
 * Create a new node in the abstract syntax tree for the cosine function
*/
struct ast_node *make_math_cos(struct ast_node *expr);

/**
 * Create a new node in the abstract syntax tree for the tangent function
*/
struct ast_node *make_math_tan(struct ast_node *expr);

/**
 * Create a new node in the abstract syntax tree for the square root function
*/
struct ast_node *make_math_sqrt(struct ast_node *expr);

/**
 * Create a new node in the abstract syntax tree for the random function
*/
struct ast_node *make_math_random(struct ast_node *lower_bound, struct ast_node *upper_bound);


// root of the abstract syntax tree
struct ast {
  struct ast_node *unit;
};

// do not forget to destroy properly! no leaks allowed!
void ast_destroy(struct ast *self);

void ast_node_destroy(struct ast_node *self);

struct array {
  size_t size;
  size_t capacity;
  struct ast_node** data;
};

struct array *array_create();
void array_destroy(struct array *self);
void array_push_back(struct array *self, struct ast_node *node);
struct ast_node *array_var_get(struct array *self, const char *name);
struct ast_node *array_proc_get(struct array *self, const char *name);

// the execution context
struct context {
  double x;
  double y;
  double angle;
  bool up;

  enum color color;

  struct array *procs;
  struct array *vars;
};

// create an initial context
void context_create(struct context *self);

void context_destroy(struct context *self);

// print the tree as if it was a Turtle program
void ast_print(const struct ast *self);

/**
 * Print the node of an abstract syntax tree
*/
void ast_node_print(const struct ast_node *self);

/**
 * Print the node of simple commands in an abstract syntax tree
*/
void ast_node_print_cmd_simple(const struct ast_node *self);

/**
 * Print the repeat command node of an abstract syntax tree
*/
void ast_node_print_cmd_repeat(const struct ast_node *self);

/**
 * Print the command block node of an abstract syntax tree
*/
void ast_node_print_cmd_block(const struct ast_node *self);

/**
 * Print the proc command node of an abstract syntax tree
*/
void ast_node_print_cmd_proc(const struct ast_node *self);

/**
 * Print the call command node of an abstract syntax tree
*/
void ast_node_print_cmd_call(const struct ast_node *self);

/**
 * Print the set command node of an abstract syntax tree
*/
void ast_node_print_cmd_set(const struct ast_node *self);

/**
 * Print the node of the expression of functions in an abstract syntax tree
*/
void ast_node_print_expr_func(const struct ast_node *self);

/**
 * Print the node of the expression of numerical values from an abstract syntax tree
*/
void ast_node_print_expr_value(const struct ast_node *self);

/**
 * Print the unary operation expression node of an abstract syntax tree
*/
void ast_node_print_expr_unop(const struct ast_node *self);

/**
 * Print the node of the expression of binary operations in an abstract syntax tree
*/
void ast_node_print_expr_binop(const struct ast_node *self);

/**
 * Print the node of expression blocks in an abstract syntax tree
*/
void ast_node_print_expr_block(const struct ast_node *self);

/**
 * Print the node of expressions of variable names and procedures of an abstract syntax tree
*/
void ast_node_print_expr_name(const struct ast_node *self);

// evaluate the tree and generate some basic primitives
void ast_eval(const struct ast *self, struct context *ctx);

/**
 * Evaluate all nodes of an abstract syntax tree
*/
double ast_eval_node(const struct ast_node *self, struct context *ctx);

/**
 * Evaluate the node of simple commands in an abstract syntax tree
*/
void ast_eval_node_cmd_simple(const struct ast_node *self, struct context *ctx);

/**
 * Evaluate the 'repeat' command node of an abstract syntax tree
*/
void ast_eval_node_cmd_repeat(const struct ast_node *self, struct context *ctx);

/**
 * Evaluate the command block nodes of an abstract syntax tree
*/
void ast_eval_node_cmd_block(const struct ast_node *self, struct context *ctx);

/**
 * Evaluate the proc command node of an abstract syntax tree
*/
void ast_eval_node_cmd_proc(const struct ast_node *self, struct context *ctx);

/**
 * Evaluate the call command node of an abstract syntax tree
*/
void ast_eval_node_cmd_call(const struct ast_node *self, struct context *ctx);

/**
 * Evaluate the set command node of an abstract syntax tree
*/
void ast_eval_node_cmd_set(const struct ast_node *self, struct context *ctx);

/**
 * Evaluate the node of the function expression in an abstract syntax tree
*/
double ast_eval_node_expr_func(const struct ast_node *self, struct context *ctx);

/**
 * Evaluate the node of the expression of numerical values in an abstract syntax tree
*/
void ast_eval_node_expr_value(const struct ast_node *self, struct context *ctx);

/**
 * Evaluate the node of the unary operation expression in an abstract syntax tree
*/
double ast_eval_node_expr_unop(const struct ast_node *self, struct context *ctx);

/**
 * Evaluate the node of the binary operations expression of an abstract syntax tree
*/
double ast_eval_node_expr_binop(const struct ast_node *self, struct context *ctx);

/**
 * Evaluate the node of expression blocks in an abstract syntax tree
*/
void ast_eval_node_expr_block(const struct ast_node *self, struct context *ctx);

/**
 * Evaluate the node of variable and procedure name expressions in an abstract syntax tree
*/
double ast_eval_node_expr_name(const struct ast_node *self, struct context *ctx);

/**
 * Evaluate the node of color command expressions in an abstract syntax tree
*/
void ast_eval_node_cmd_color(const struct ast_node *self, struct context *ctx);

#endif /* TURTLE_AST_H */
