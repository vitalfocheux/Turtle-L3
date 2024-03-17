%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "turtle-ast.h"
#include "turtle-parser.h"
%}

%option warn 8bit nodefault noyywrap

COMMENT         "#".*
DIGIT           [0-9]
DOUBLE          ([0-9]+\.?|[0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?
HEXA            0[xX][0-9a-fA-F]+
BINAIRE         0[bB][01]+
VAR             [A-Z][A-Z0-9]*

%%

    /* les reconnaissances des lexèmes relatif aux valeurs reconnu avec des regex */
{DIGIT}+              { yylval.value = strtod(yytext, NULL); return VALUE; }
{DOUBLE}+             { yylval.value = strtod(yytext, NULL); return VALUE; }
{HEXA}+               { yylval.value = strtod(yytext, NULL); return VALUE; }
{VAR}                 { yylval.name = yytext; return NAME; }

"red"                 {return RED;}
"green"               {return GREEN;}
"blue"                {return BLUE;}
"cyan"                {return CYAN;}
"magenta"             {return MAGENTA;}
"yellow"              {return YELLOW;}
"black"               {return BLACK;}
"grey"                {return GREY;}
"white"               {return WHITE;}

    /* les reconnaissances des lexèmes relatif aux fonctions */

"sin"                 { return FCT_SIN; }
"cos"                 { return FCT_COS; }
"tan"                 { return FCT_TAN; }
"sqrt"                { return FCT_SQRT; }
"random"              { return FCT_RANDOM; }

    /* les reconnaissances des lexèmes relatif aux commandes */

"print"               { return KW_PRINT; }
"up"                  { return KW_UP; }
"down"                { return KW_DOWN; }
"forward"             { return KW_FORWARD; }
"fw"                  { return KW_FORWARD; }
"backward"            { return KW_BACKWARD; }
"bw"                  { return KW_BACKWARD; }
"position"            { return KW_POSITION; }
"pos"                 { return KW_POSITION; }
"right"               { return KW_RIGHT; }
"rt"                  { return KW_RIGHT; }
"left"                { return KW_LEFT; }
"lt"                  { return KW_LEFT; }
"heading"             { return KW_HEADING; }
"hd"                  { return KW_HEADING; }
"color"               { return KW_COLOR; }
"home"                { return KW_HOME; }

"repeat"              { return KW_REPEAT; }

"set"                 { return KW_SET; }

"proc"                { return KW_PROC; }
"call"                { return KW_CALL; }

[-+*/^,(){}]          { return *yytext; }

{COMMENT}             /* commentaire */
[\n\t ]*              /* whitespace */
.                     { fprintf(stderr, "Unknown token: '%s'\n", yytext); exit(EXIT_FAILURE); }

%%
