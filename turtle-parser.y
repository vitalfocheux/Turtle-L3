%{
#include <stdio.h>

#include "turtle-ast.h"

int yylex();
void yyerror(struct ast *ret, const char *);

%}

%debug
%defines

%define parse.error verbose

%parse-param { struct ast *ret }

%union {
  double value;
  char *name;
  char *color;
  struct ast_node *node;
}

%token <value>    VALUE       "value"
%token <name>     NAME        "name"
%token <color>    COLOR       "color"

%token            FCT_SIN     
%token            FCT_COS     
%token            FCT_TAN     
%token            FCT_SQRT    
%token            FCT_RANDOM  

%token            KW_PRINT    
%token            KW_UP       
%token            KW_DOWN     
%token            KW_FORWARD  
%token            KW_BACKWARD 
%token            KW_POSITION 
%token            KW_RIGHT    
%token            KW_LEFT    
%token            KW_HEADING  
%token            KW_COLOR    
%token            KW_HOME     

%token            KW_REPEAT   

%token            KW_SET      

%token            KW_PROC     
%token            KW_CALL     


/* TODO: add other tokens */

%left '+' '-'
%left '*' '/'
%left '^'
%left UNARY_OP

%type <node> unit cmds cmd expr func

%%

unit:
    cmds              { $$ = $1; ret->unit = $$; }
;

cmds:
    cmd cmds          { $1->next = $2; $$ = $1; }
  | /* empty */       { $$ = NULL; }
;

cmd:
    KW_PRINT    expr                    { $$ = make_cmd_print($2); }
  | KW_UP                               { $$ = make_cmd_up(); }
  | KW_DOWN                             { $$ = make_cmd_down(); }
  | KW_FORWARD  expr                    { $$ = make_cmd_forward($2); }
  | KW_BACKWARD expr                    { $$ = make_cmd_backward($2); }
  | KW_POSITION expr ',' expr           { $$ = make_cmd_pos($2, $4); }
  | KW_RIGHT    expr                    { $$ = make_cmd_right($2); }
  | KW_LEFT     expr                    { $$ = make_cmd_left($2); }
  | KW_HEADING  expr                    { $$ = make_cmd_heading($2); }
  | KW_COLOR    expr ',' expr ',' expr  { $$ = make_cmd_color_RGB($2, $4, $6); }
  | KW_COLOR    COLOR                   { $$ = make_cmd_color($2); }
  | KW_HOME                             { $$ = make_cmd_home(); }
  | KW_REPEAT   expr '{' expr '}'       { $$ = make_cmd_repeat($2, $4); }
  | KW_SET      NAME     expr           { $$ = make_cmd_set($2, $3); }
  | KW_PROC     NAME '{' cmd  '}'       { $$ = make_cmd_proc($2, $4); }
  | KW_CALL     NAME                    { $$ = make_cmd_call($2); }
;

expr:
    VALUE             { $$ = make_expr_value($1); }
  | NAME              { $$ = make_expr_name($1); }
  | expr '+' expr    { $$ = make_expr_binop('+', $1, $3); }
  | expr '-' expr    { $$ = make_expr_binop('-', $1, $3); }
  | expr '*' expr    { $$ = make_expr_binop('*', $1, $3); }
  | expr '/' expr    { $$ = make_expr_binop('/', $1, $3); }
  | expr '^' expr    { $$ = make_expr_binop('^', $1, $3); }
  | UNARY_OP expr    { $$ = make_expr_unop('-', $2); }
  | func
;

func:
    FCT_SIN     '(' expr ')'               { $$ = make_math_sin($3); }
  | FCT_COS     '(' expr ')'               { $$ = make_math_cos($3); }
  | FCT_TAN     '(' expr ')'               { $$ = make_math_tan($3); }
  | FCT_SQRT    '(' expr ')'               { $$ = make_math_sqrt($3); }
  | FCT_RANDOM  '(' expr ',' expr ')'      { $$ = make_math_random($3, $5); }
;

%%

void yyerror(struct ast *ret, const char *msg) {
  (void) ret;
  fprintf(stderr, "%s\n", msg);
}
