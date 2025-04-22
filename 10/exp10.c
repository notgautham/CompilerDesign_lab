#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INSTRUCTIONS 100
#define MAX_EXPR_LEN 100

// Structures for Intermediate Representations

// Quadruple representation: (Operator, Operand1, Operand2, Result)
typedef struct {
    char op[5];
    char arg1[5];
    char arg2[5];
    char result[5];
} Quadruple;

// Triple representation: (Operator, Operand1, Operand2)
typedef struct {
    char op[5];
    char arg1[5];
    char arg2[5];
} Triple;

// Indirect Triple representation: Pointer-based indexing for triples
typedef struct {
    int index;
    char op[5];
    char arg1[5];
    char arg2[5];
} IndirectTriple;

// Global arrays to store intermediate representations
Quadruple quad[MAX_INSTRUCTIONS];
Triple triple[MAX_INSTRUCTIONS];
IndirectTriple indirectTriple[MAX_INSTRUCTIONS];
int instruction_count = 0;
int temp_count = 1;

// Function to add a quadruple instruction
void add_quad(char *op, char *arg1, char *arg2, char *result) {
    strcpy(quad[instruction_count].op, op);
    strcpy(quad[instruction_count].arg1, arg1);
    strcpy(quad[instruction_count].arg2, arg2);
    strcpy(quad[instruction_count].result, result);
    instruction_count++;
}

// Returns the precedence of the operator
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Generate intermediate code from the given arithmetic expression
void generate_intermediate_code() {
    char expr[MAX_EXPR_LEN];
    printf("Please input an arithmetic expression: ");
    fgets(expr, MAX_EXPR_LEN, stdin);
    expr[strcspn(expr, "\n")] = 0;  // Remove newline

    // Rename stacks for clarity
    char operandStack[MAX_EXPR_LEN][5];
    int operandTop = -1;
    char operatorStack[MAX_EXPR_LEN];
    int operatorTop = -1;

    // Parse each character of the expression
    for (int i = 0; i < strlen(expr); i++) {
        if (isspace(expr[i]))
            continue;

        if (isalnum(expr[i])) {  // If operand found, push it on operandStack
            char operand[5] = {expr[i], '\0'};
            strcpy(operandStack[++operandTop], operand);
        } else if (expr[i] == '(') {
            operatorStack[++operatorTop] = expr[i];
        } else if (expr[i] == ')') {
            // Process until matching '(' is found
            while (operatorTop >= 0 && operatorStack[operatorTop] != '(') {
                char op[5] = {operatorStack[operatorTop--], '\0'};
                char arg2[5], arg1[5], result[5];
                strcpy(arg2, operandStack[operandTop--]);
                strcpy(arg1, operandStack[operandTop--]);
                sprintf(result, "t%d", temp_count++);
                add_quad(op, arg1, arg2, result);
                strcpy(operandStack[++operandTop], result);
            }
            operatorTop--; // Discard '('
        } else { // If an operator is encountered
            while (operatorTop >= 0 && precedence(operatorStack[operatorTop]) >= precedence(expr[i])) {
                char op[5] = {operatorStack[operatorTop--], '\0'};
                char arg2[5], arg1[5], result[5];
                strcpy(arg2, operandStack[operandTop--]);
                strcpy(arg1, operandStack[operandTop--]);
                sprintf(result, "t%d", temp_count++);
                add_quad(op, arg1, arg2, result);
                strcpy(operandStack[++operandTop], result);
            }
            operatorStack[++operatorTop] = expr[i];
        }
    }

    // Process any remaining operators on the stack
    while (operatorTop >= 0) {
        char op[5] = {operatorStack[operatorTop--], '\0'};
        char arg2[5], arg1[5], result[5];
        strcpy(arg2, operandStack[operandTop--]);
        strcpy(arg1, operandStack[operandTop--]);
        sprintf(result, "t%d", temp_count++);
        add_quad(op, arg1, arg2, result);
        strcpy(operandStack[++operandTop], result);
    }
}

// Display the generated quadruple instructions
void display_quadruples() {
    printf("\nGenerated Quadruple Representation:\n");
    printf("------------------------------------------------\n");
    printf("Operator\tOperand1\tOperand2\tResult\n");
    for (int i = 0; i < instruction_count; i++) {
        printf("%s\t\t%s\t\t%s\t\t%s\n", quad[i].op, quad[i].arg1, quad[i].arg2, quad[i].result);
    }
}

// Create triple representation based on quadruple instructions
void generate_triples() {
    for (int i = 0; i < instruction_count; i++) {
        strcpy(triple[i].op, quad[i].op);
        strcpy(triple[i].arg1, quad[i].arg1);
        strcpy(triple[i].arg2, quad[i].arg2);
    }
}

// Display the triple representation
void display_triples() {
    printf("\nGenerated Triple Representation:\n");
    printf("-------------------------------------------------\n");
    printf("Index\tOperator\tOperand1\tOperand2\n");
    for (int i = 0; i < instruction_count; i++) {
        printf("(%d)\t%s\t\t%s\t\t%s\n", i, triple[i].op, triple[i].arg1, triple[i].arg2);
    }
}

// Create indirect triple representation from the triples
void generate_indirect_triples() {
    for (int i = 0; i < instruction_count; i++) {
        indirectTriple[i].index = i;
        strcpy(indirectTriple[i].op, triple[i].op);
        strcpy(indirectTriple[i].arg1, triple[i].arg1);
        strcpy(indirectTriple[i].arg2, triple[i].arg2);
    }
}

// Display the indirect triple representation
void display_indirect_triples() {
    printf("\nIndirect Triple Representation:\n");
    printf("-------------------------------------------------\n");
    printf("Pointer\tOperator\tOperand1\tOperand2\n");
    for (int i = 0; i < instruction_count; i++) {
        printf("(%d)\t%s\t\t%s\t\t%s\n", indirectTriple[i].index, indirectTriple[i].op, indirectTriple[i].arg1, indirectTriple[i].arg2);
    }
}

// Main function
int main() {
    generate_intermediate_code();
    display_quadruples();
    generate_triples();
    display_triples();
    generate_indirect_triples();
    display_indirect_triples();

    return 0;
}
