%{
    #include<stdio.h>
    #include<stdlib.h>
    #include "0348_calculator.tab.h"
%}

%%

[0-9]+    { yylval.int_val = atoi(yytext); return INTEGER; }

[0-9]+\.[0-9]+    { yylval.float_val = atof(yytext); return FLOAT; }  // Changed to atof()

"+"    { return PLUS; }
"-"    { return MINUS; }
"*"    { return MULTIPLY; }
"/"    { return DIVIDE; }
"**"   { return POWER; }
"("    { return LPAREN; }
")"    { return RPAREN; }
"clear" { return CLEAR; }

[ \t]    ;  // Ignore whitespace
\n       { return '\n'; }

.        { printf("Invalid character: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}
