#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 100 // Maximum stack size

// Stack structure for storing expressions
typedef struct {
    char items[MAX][MAX]; // Stack to store operands and intermediate results
    int top; // Top of the stack
} Stack;

// Initialize stack
void initStack(Stack *s) { s->top = -1; }

// Push an element onto the stack
void push(Stack *s, char *str) { if (s->top < MAX - 1) strcpy(s->items[++s->top], str); }

// Pop an element from the stack
char *pop(Stack *s) { return (s->top == -1) ? "" : s->items[s->top--]; }

// Function to determine the precedence of an operator
int precedence(char op) { return (op == '+' || op == '-') ? 1 : (op == '*' || op == '/') ? 2 : (op == '^') ? 3 : 0; }

// Convert an infix expression to postfix notation
void infixToPostfix(char *infix, char *postfix) {
    Stack s; initStack(&s); int i, j = 0;
    for (i = 0; infix[i] != '\0'; i++) {
        if (isalnum(infix[i])) postfix[j++] = infix[i];
        else if (infix[i] == '(') push(&s, (char[]){infix[i], '\0'});
        else if (infix[i] == ')') {
            while (s.top != -1 && s.items[s.top][0] != '(') postfix[j++] = pop(&s)[0];
            pop(&s); // Pop '('
        } else {
            while (s.top != -1 && precedence(s.items[s.top][0]) >= precedence(infix[i])) 
                postfix[j++] = pop(&s)[0];
            push(&s, (char[]){infix[i], '\0'});
        }
    }
    while (s.top != -1) postfix[j++] = pop(&s)[0];
    postfix[j] = '\0';
}

// Convert an infix expression to prefix notation
void infixToPrefix(char *infix, char *prefix) {
    int len = strlen(infix); char temp[MAX], tempPrefix[MAX];
    for (int i = len - 1, j = 0; i >= 0; i--) {
        temp[j++] = (infix[i] == '(') ? ')' : (infix[i] == ')') ? '(' : infix[i];
    }
    temp[len] = '\0'; infixToPostfix(temp, tempPrefix); 
    for (int i = 0, j = strlen(tempPrefix) - 1; j >= 0; i++, j--) prefix[i] = tempPrefix[j];
    prefix[strlen(tempPrefix)] = '\0';
}

// Generate intermediate code (Three Address Code - TAC) from postfix notation
void generateTAC(char *postfix) {
    Stack s; initStack(&s); int tCount = 1;
    printf("\nThree Address Code (TAC):\n");
    for (int i = 0; postfix[i] != '\0'; i++) {
        if (isalnum(postfix[i])) push(&s, (char[]){postfix[i], '\0'});
        else {
            char operand2[MAX], operand1[MAX], result[MAX];
            strcpy(operand2, pop(&s)); strcpy(operand1, pop(&s));
            sprintf(result, "T%d", tCount++);
            printf("%s = %s %c %s\n", result, operand1, postfix[i], operand2);
            push(&s, result);
        }
    }
}

// Main function
int main() {
    char infix[MAX], postfix[MAX], prefix[MAX];
    printf("Enter an infix expression: ");
    scanf("%s", infix);
    infixToPostfix(infix, postfix); printf("\nPostfix Expression: %s\n", postfix);
    infixToPrefix(infix, prefix); printf("Prefix Expression: %s\n", prefix);
    generateTAC(postfix);
    return 0;
}