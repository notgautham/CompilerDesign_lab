%{
#include <stdio.h>
#include <string.h>
int line_count = 0;
int word_count = 0;
int space_count = 0;
int tab_count = 0;
int word_frequency = 0;
char search_word[100];
%}

%% 
\n        { line_count++; }
[ \t]     { space_count++; tab_count++; }
[a-zA-Z]+ { word_count++; 
            if (strcmp(yytext, search_word) == 0) word_frequency++; }
.         { /* ignore other characters */ }

%%
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <file> <word>\n", argv[0]);
        return 1;
    }

    strcpy(search_word, argv[2]);

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        printf("Error opening file %s\n", argv[1]);
        return 1;
    }

    yyin = input_file;
    yylex();

    fclose(input_file);

    printf("Lines: %d\n", line_count+1);
    printf("Words: %d\n", word_count);
    printf("Spaces: %d\n", space_count);
    printf("Tab spaces: %d\n", tab_count);
    printf("Frequency of '%s': %d\n", search_word, word_frequency);
    return 0;
}

int yywrap() {
return 1;
}
