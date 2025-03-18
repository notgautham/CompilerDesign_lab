# Compiler Design Lab

This repository contains the code and documentation for the Compiler Design Lab course. The course involves implementing various concepts in compiler construction, including lexical analysis, parsing, code generation, and optimizations. Below is a list of experiments covered so far in the course:

## Experiments

### 1. Simple Calculator
- Implemented a basic calculator using **Flex** and **Bison**.

### 2. Lexical Analyzer using FLEX
- Developed a lexical analyzer using the **FLEX** tool to break down and identify tokens in source code.

### 3. Simple Calculator using FLEX and BISON
- Built a more advanced calculator that uses **FLEX** for lexical analysis and **BISON** for syntax analysis.

### 4. Elimination of Left Recursion/Left Factoring
- Implemented algorithms to eliminate left recursion and apply left factoring in context-free grammars.

### 5. Computation of FIRST & FOLLOW (Top-Down Parsing)
- Computed the **FIRST** and **FOLLOW** sets for a given grammar and generated the predictive parsing table.

### 6. Shift Reduce Parsing
- Implemented a **Shift-Reduce** parsing technique to analyze the syntax of a language.

### 7. Computation of LEADING AND TRAILING
- Computed the **LEADING** and **TRAILING** sets, which are essential for syntax analysis in some parsing techniques.

## Future Work
This repository will be updated with additional experiments as we progress through the Compiler Design Lab course.

Feel free to explore the individual experiment directories for more details and implementations. Each experiment contains the required code and documentation to understand and run the program.

---

### How to Run the Code

To run the experiments, make sure you have the following tools installed:
- **Flex**: A tool for lexical analysis.
- **Bison**: A parser generator for generating parsers.
- **GCC**: A C compiler for compiling the generated code.

Clone this repository and navigate to the experiment directory to compile and execute the code.

```bash
git clone https://github.com/yourusername/compiler-design-lab.git
cd experiment-directory
make
./executable
