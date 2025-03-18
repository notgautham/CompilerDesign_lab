%{
#include <stdio.h>
#include <math.h>
%}

%%

"int"|"float"|"char"|"if"|"else"|"for"|"while"|"return"  { printf("Keyword: %s\n", yytext); } 

[0-9]+                                      { printf("Constant: %s\n", yytext); }
[0-9]+\.[0-9]+                              { printf("Float Constant: %s\n", yytext); }
[0-9]+"L"|"0x[0-9a-fA-F]+"                  { printf("Special Constant: %s\n", yytext); }
[0-9]+[Ee][+-]?[0-9]+                       { printf("Scientific Notation: %s\n", yytext); }


[0-9][a-zA-Z_]*                             { printf("Variable with Register Prefix: %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*                     { printf("Identifier: %s\n", yytext); }


[+\-*/=><]                                 { printf("Operator: %s\n", yytext); }
[{}();]                                    { printf("Symbol: %s\n", yytext); }

[ \t\n]+                                   { /* Ignore whitespace */ }

.                                          { printf("Unrecognized token: %s\n", yytext); }

%%

int yywrap() { return 1; }

int main() {
    char *code = 
        "0348int a = 10;          \n"
        "0348float b = 20.5;      \n"
        "0348char c = 'x';        \n"
        "if (a < b) {         \n"
        "    a = a + 5;       \n"
        "} else {             \n"
        "    b = b - 3.5;     \n"
        "}                    \n"
        "for (int i = 0; i < 5; i++) { \n"
        "    printf(\"i = %d\\n\", i); \n"
        "}                    \n"
        "return 0;            \n";

    yy_scan_string(code); // Pass the hardcoded string to FLEX
    yylex();              // Perform lexical analysis

    return 0;
}
