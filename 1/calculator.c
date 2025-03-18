#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function prototype to evaluate an expression
// Takes a string expression and an error flag
// Returns the computed result
double eval_expr(const char *expr, int *err);

// Function to parse numbers and handle parentheses
double parse_factor(const char **expr, int *err) {
    double result = 0;
    
    if (**expr == '(') { // If an opening parenthesis is encountered
        (*expr)++; // Move past '('
        result = eval_expr(*expr, err); // Recursively evaluate the expression inside parentheses
        if (**expr != ')') { // If no matching closing parenthesis
            *err = 1; // Set error flag
            return 0;
        }
        (*expr)++; // Move past ')'
    } else {
        result = strtod(*expr, (char **)expr); // Convert number from string to double
    }
    return result;
}

// Function to handle multiplication and division
double parse_term(const char **expr, int *err) {
    double result = parse_factor(expr, err);
    
    while (**expr == '*' || **expr == '/') { // Check for multiplication or division
        char op = *(*expr)++; // Store the operator and move to the next character
        double next = parse_factor(expr, err); // Parse the next number
        result = (op == '*') ? result * next : result / next; // Perform the operation
    }
    return result;
}

// Function to evaluate an expression with addition and subtraction
double eval_expr(const char *expr, int *err) {
    double result = parse_term(&expr, err);
    
    while (*expr == '+' || *expr == '-') { // Check for addition or subtraction
        char op = *expr++; // Store the operator and move to the next character
        double next = parse_term(&expr, err); // Parse the next term
        result = (op == '+') ? result + next : result - next; // Perform the operation
    }
    
    return *err ? 0 : result; // Return result, or 0 if there was an error
}

int main() {
    char expr[100]; // Buffer to hold user input
    
    while (fgets(expr, sizeof(expr), stdin)) { // Read an expression from standard input
        int err = 0; // Error flag
        double result = eval_expr(expr, &err); // Evaluate the expression
        
        if (err) printf("Error: Unmatched parentheses\n"); // Print error message if parentheses are incorrect
        else printf("Result: %.6f\n", result); // Print the computed result
    }
    return 0; // Exit the program
}
