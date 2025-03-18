%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>

    void yyerror(const char *s);
    int yylex(void);
%}

%union {
    int int_val;
    double float_val;
}

%token <int_val> INTEGER
%token <float_val> FLOAT
%token PLUS MINUS MULTIPLY DIVIDE POWER
%token LPAREN RPAREN CLEAR

%type <float_val> expr
%left PLUS MINUS
%left MULTIPLY DIVIDE
%right POWER

%%

program:
      | program expr '\n'    { printf("Results: %lf\n", $2); }
      | program CLEAR '\n'   { system("cls"); }

expr:
      INTEGER    { $$ = (double) $1; }
    | FLOAT      { $$ = $1; }
    | expr PLUS expr    { $$ = $1 + $3; }
    | expr MINUS expr   { $$ = $1 - $3; }
    | expr MULTIPLY expr { $$ = $1 * $3; }
    | expr DIVIDE expr   {
        if ($3 == 0) {
            yyerror("Division by zero!");
            YYABORT;
        }
        $$ = $1 / $3;
    }
    | expr POWER expr   { $$ = pow($1, $3); }
    | LPAREN expr RPAREN { $$ = $2; }

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Enter the Expression (press Ctrl+Z to exit)\n");
    return yyparse();
}
