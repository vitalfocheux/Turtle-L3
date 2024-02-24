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
  struct ast_node *node;
}

%token <value>    VALUE       "value"
%token <name>     NAME        "name"

%token            RED          "red"
%token            GREEN        "green"
%token            BLUE         "blue"
%token            CYAN         "cyan"
%token            MAGENTA      "magenta"
%token            YELLOW       "yellow"
%token            BLACK        "black"
%token            GREY         "grey"
%token            WHITE        "white"

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

%type <node> unit cmds cmd expr func color param name 

%%

unit:
    cmds              { $$ = $1; ret->unit = $$; }
;

cmds:
    cmd cmds          { $1->next = $2; $$ = $1; }
  | /* empty */       { $$ = NULL; }
;

color:
    RED               { $$ = make_cmd_color_name(COLOR_RED); }
  | GREEN             { $$ = make_cmd_color_name(COLOR_GREEN); }
  | BLUE              { $$ = make_cmd_color_name(COLOR_BLUE); }
  | CYAN              { $$ = make_cmd_color_name(COLOR_CYAN); }
  | MAGENTA           { $$ = make_cmd_color_name(COLOR_MAGENTA); }
  | YELLOW            { $$ = make_cmd_color_name(COLOR_YELLOW); }
  | BLACK             { $$ = make_cmd_color_name(COLOR_BLACK); }
  | GREY              { $$ = make_cmd_color_name(COLOR_GREY); }
  | WHITE             { $$ = make_cmd_color_name(COLOR_WHITE); }
;

name:
    NAME             { $$ = make_expr_name($1); }
;

param:
    cmd              { $$ = $1; }
  | expr             { $$ = $1; }
  | name             { $$ = $1; }
;

cmd:
    KW_PRINT    param                    { $$ = make_cmd_print($2); }
  | KW_UP                               { $$ = make_cmd_up(); }
  | KW_DOWN                             { $$ = make_cmd_down(); }
  | KW_FORWARD  param                    { $$ = make_cmd_forward($2); }
  | KW_BACKWARD param                    { $$ = make_cmd_backward($2); }
  | KW_POSITION param ',' param           { $$ = make_cmd_pos($2, $4); }
  | KW_RIGHT    param                    { $$ = make_cmd_right($2); }
  | KW_LEFT     param                    { $$ = make_cmd_left($2); }
  | KW_HEADING  param                    { $$ = make_cmd_heading($2); }
  | KW_COLOR    color                   { $$ = make_cmd_color($2); }
  | KW_COLOR    param ',' param ',' param  { $$ = make_cmd_color_RGB($2, $4, $6); }
  | KW_HOME                             { $$ = make_cmd_home(); }
  | KW_REPEAT   param '{' cmds '}'         { $$ = make_cmd_repeat($2, $4); }
  | KW_SET      name     expr           { $$ = make_cmd_set($2, $3); }
  | KW_PROC     name '{' cmds '}'           { $$ = make_cmd_proc($2, $4); }
  | KW_CALL     name                    { $$ = make_cmd_call($2); }
  | '{' cmds '}'                        { $$ = make_cmd_block($2); }
;





expr:
    VALUE             { $$ = make_expr_value($1); }
  | name              { $$ = $1; }
  | expr '+' expr    { $$ = make_expr_binop('+', $1, $3); }
  | expr '-' expr    { $$ = make_expr_binop('-', $1, $3); }
  | expr '*' expr    { $$ = make_expr_binop('*', $1, $3); }
  | expr '/' expr    { $$ = make_expr_binop('/', $1, $3); }
  | expr '^' expr    { $$ = make_expr_binop('^', $1, $3); }
  | '(' expr ')'      { $$ = make_expr_block($2); }
  | '-' expr %prec UNARY_OP    { $$ = make_expr_unop('-', $2); }
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
