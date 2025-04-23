#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

// Core data structures
struct Instruction { int index; string code; };
struct BoolExpr { vector<int> trueList, falseList; };
struct Stmt { vector<int> nextList; };

// Code emitter with backpatching
class CodeEmitter {
    vector<Instruction> instructions;
    int instrCount = 0;
public:
    inline int getNextInstr() const { return instrCount; }
    
    inline void emit(const string& code) {
        instructions.push_back({instrCount++, code});
    }
    
    void backpatch(const vector<int>& list, int target) {
        for (const int& i : list) {
            if (i >= 0 && i < static_cast<int>(instructions.size())) {
                size_t pos = instructions[i].code.find('_');
                if (pos != string::npos) {
                    size_t endPos = instructions[i].code.find('_', pos + 1);
                    if (endPos != string::npos) {
                        // Use string replacement instead of creating new strings
                        instructions[i].code.replace(pos, endPos - pos + 1, to_string(target));
                    }
                }
            }
        }
    }
    
    void print() const {
        for (const auto& instr : instructions)
            cout << instr.index << ": " << instr.code << endl;
    }
};

// Forward declarations
inline BoolExpr generateCondition(CodeEmitter& e, const string& cond);
inline BoolExpr parseExpression(CodeEmitter& e, const string& expr);

// Utility to merge lists - made inline and uses references
inline void merge(vector<int>& result, const vector<int>& a, const vector<int>& b) {
    result = a;
    result.insert(result.end(), b.begin(), b.end());
}

// Code generation for simple boolean condition
inline BoolExpr generateCondition(CodeEmitter& e, const string& cond) {
    BoolExpr b;
    // Use stringstream as a string builder
    stringstream ss;
    ss << "if " << cond << " goto _" << e.getNextInstr() + 1 << "_";
    e.emit(ss.str());
    b.trueList.push_back(e.getNextInstr() - 1);
    
    ss.str(""); // Clear the stringstream
    ss << "goto _" << e.getNextInstr() + 1 << "_";
    e.emit(ss.str());
    b.falseList.push_back(e.getNextInstr() - 1);
    return b;
}

// Parse and generate code for complex boolean expressions
inline BoolExpr parseExpression(CodeEmitter& e, const string& expr) {
    // Check for OR operation
    size_t orPos = expr.find("||");
    if (orPos != string::npos) {
        BoolExpr leftExpr = parseExpression(e, expr.substr(0, orPos));
        e.backpatch(leftExpr.falseList, e.getNextInstr());
        BoolExpr rightExpr = parseExpression(e, expr.substr(orPos + 2));
        
        BoolExpr result;
        merge(result.trueList, leftExpr.trueList, rightExpr.trueList);
        result.falseList = rightExpr.falseList;
        return result;
    }
    
    // Check for AND operation
    size_t andPos = expr.find("&&");
    if (andPos != string::npos) {
        BoolExpr leftExpr = parseExpression(e, expr.substr(0, andPos));
        e.backpatch(leftExpr.trueList, e.getNextInstr());
        BoolExpr rightExpr = parseExpression(e, expr.substr(andPos + 2));
        
        BoolExpr result;
        result.trueList = rightExpr.trueList;
        merge(result.falseList, leftExpr.falseList, rightExpr.falseList);
        return result;
    }
    
    // Simple condition
    return generateCondition(e, expr);
}

// Generate code for a for loop
inline void generateForLoop(CodeEmitter& e, const string& init, const string& cond, 
                     const string& update, const string& body) {
    e.emit(init);
    int condLabel = e.getNextInstr();
    
    BoolExpr bExpr = parseExpression(e, cond);
    e.backpatch(bExpr.trueList, e.getNextInstr());
    
    e.emit(body);
    e.emit(update);
    
    // Use stringstream instead of string concatenation
    stringstream ss;
    ss << "goto _" << condLabel << "_";
    e.emit(ss.str());
    
    e.backpatch(bExpr.falseList, e.getNextInstr());
}

// Generate code for an if-else statement
inline void generateIfElse(CodeEmitter& e, const string& cond, const string& thenCode, 
                   const string& elseCode) {
    BoolExpr bExpr = parseExpression(e, cond);
    
    e.backpatch(bExpr.trueList, e.getNextInstr());
    e.emit(thenCode);
    
    int gotoEnd = e.getNextInstr();
    // Use stringstream instead of string concatenation
    stringstream ss;
    ss << "goto _" << e.getNextInstr() + 2 << "_";
    e.emit(ss.str());
    
    e.backpatch(bExpr.falseList, e.getNextInstr());
    e.emit(elseCode);
    
    e.backpatch({gotoEnd}, e.getNextInstr());
}

int main() {
    CodeEmitter emitter;
    int choice;
    
    cout << "Choose structure (1: Boolean Expression, 2: if-else, 3: for-loop): ";
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        string condition;
        cout << "Enter Boolean condition (e.g., x > 5 || y < 10): ";
        getline(cin, condition);

        BoolExpr cond = parseExpression(emitter, condition);
        
        emitter.backpatch(cond.trueList, emitter.getNextInstr());
        emitter.emit("// True path");
        
        int gotoEnd = emitter.getNextInstr();
        // Use stringstream instead of string concatenation
        stringstream ss;
        ss << "goto _" << emitter.getNextInstr() + 2 << "_";
        emitter.emit(ss.str());
        
        emitter.backpatch(cond.falseList, emitter.getNextInstr());
        emitter.emit("// False path");
        
        emitter.backpatch({gotoEnd}, emitter.getNextInstr());
    } 
    else if (choice == 2) {
        string condition, thenCode, elseCode;
        cout << "Enter condition: ";
        getline(cin, condition);
        cout << "Enter THEN statement: ";
        getline(cin, thenCode);
        cout << "Enter ELSE statement: ";
        getline(cin, elseCode);

        generateIfElse(emitter, condition, thenCode, elseCode);
    } 
    else if (choice == 3) {
        string init, condition, update, body;
        cout << "Enter initialization: ";
        getline(cin, init);
        cout << "Enter condition: ";
        getline(cin, condition);
        cout << "Enter update: ";
        getline(cin, update);
        cout << "Enter body code: ";
        getline(cin, body);

        generateForLoop(emitter, init, condition, update, body);
    }
    else {
        cout << "Invalid choice!" << endl;
        return 1;
    }

    cout << "\nGenerated code with backpatching:\n" << endl;
    emitter.print();
    return 0;
}