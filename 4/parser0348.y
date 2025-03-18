%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void yyerror(const char *s);
int yylex();
int yyparse();
%}

/* Declare the data types used for tokens and non-terminals */
%union {
    float fval;
    char *sval;
}

/* Token declarations with types */
%token <fval> NUMBER
%token <sval> ID
%token ASSIGN POWER

/* Declare types for non-terminals */
%type <fval> expr term factor base

/* Operator precedence and associativity */
%right POWER  /* Exponentiation is right-associative */
%left '*' '/' /* Multiplication and division */
%left '+' '-' /* Addition and subtraction */

%%

stmt: 
      expr '\n' { printf("Result: %f\n", $1); fflush(stdout); }  /* Correct newline handling */
    | '\n'  /* Allows empty lines to prevent syntax errors */
    ;

expr:
      term { $$ = $1; }
    | expr '+' term { $$ = $1 + $3; }
    | expr '-' term { $$ = $1 - $3; }
    ;

term:
      factor { $$ = $1; }
    | term '*' factor { $$ = $1 * $3; }
    | term '/' factor { $$ = $1 / $3; }
    ;

factor:
      base { $$ = $1; }
    | factor POWER base { $$ = pow($1, $3); }
    ;

base:
      NUMBER { $$ = $1; }
    | '(' expr ')' { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Enter expressions (Press Ctrl+D to exit on Linux/macOS, Ctrl+Z on Windows):\n");
    fflush(stdout);
    if (yyparse() == 0) {  // yyparse() returns 0 on success, non-zero on failure
        printf("\nExiting...\n");
    }
    return 0;
}
