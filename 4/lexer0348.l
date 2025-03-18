%{
#include "parser0348.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  /* Required for strdup() */
%}

%%

[0-9]+(\.[0-9]+)?   { yylval.fval = atof(yytext); return NUMBER; }
[+\-*/()]           { return yytext[0]; }
"**"                { return POWER; }
"="                 { return ASSIGN; }
[a-zA-Z][a-zA-Z0-9]* { yylval.sval = strdup(yytext); return ID; } /* strdup now works */
[ \t]+              { /* Ignore spaces and tabs */ }
\n                  { return '\n'; }  /* Ensure expressions are processed line by line */
.                   { printf("Unrecognized character: %s\n", yytext); fflush(stdout); }

%%

int yywrap() {
    return 1;
}
