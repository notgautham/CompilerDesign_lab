#include <iostream>
#include <stack>
#include <vector>
#include <string>

using namespace std;

// Grammar Rules
string rules[] = {"E+E", "E*E", "(E)", "id"};
string replaceWith = "E";  // Non-terminal symbol to replace valid expressions

// Function to check if a given string matches any grammar rule
bool matchRule(string str) {
    for (string rule : rules) {
        if (str == rule) {
            return true;
        }
    }
    return false;
}

// Function to perform Shift Reduce Parsing
void shiftReduceParser(vector<string> input) {
    stack<string> parseStack;
    int i = 0;

    cout << "Step-by-step Shift Reduce Parsing:\n\n";
    while (i < input.size() || parseStack.size() > 1) {
        // Print current stack contents
        cout << "Stack: ";
        stack<string> tempStack = parseStack;
        vector<string> tempVec;
        while (!tempStack.empty()) {
            tempVec.insert(tempVec.begin(), tempStack.top());
            tempStack.pop();
        }
        for (string s : tempVec) cout << s << " ";
        cout << "\n";

        // Shift Operation (Move input to stack)
        if (i < input.size()) {
            cout << "Shift: " << input[i] << "\n";
            parseStack.push(input[i]);
            i++;
        }

        // Reduce Operation (Check for matching rule)
        while (true) {
            string stackTop = "";
            stack<string> temp = parseStack;
            vector<string> checkVec;
            while (!temp.empty()) {
                checkVec.insert(checkVec.begin(), temp.top());
                temp.pop();
            }

            // Try matching longest possible rule
            for (int j = 1; j <= checkVec.size(); j++) {
                string candidate = "";
                for (int k = checkVec.size() - j; k < checkVec.size(); k++) {
                    candidate += checkVec[k];
                }

                if (matchRule(candidate)) {
                    cout << "Reduce: " << candidate << " → E\n";
                    for (int k = 0; k < j; k++) {
                        parseStack.pop();
                    }
                    parseStack.push(replaceWith);
                    break;  // Restart checking after reduction
                }
            }

            // Stop reduction if no rule is matched
            if (parseStack.top() == "E") {
                break;
            }
        }

        cout << "------------------------\n";
    }

    // Final Check
    if (parseStack.size() == 1 && parseStack.top() == "E") {
        cout << "Parsing successful! The input string is valid.\n";
    } else {
        cout << "Parsing failed! The input string is invalid.\n";
    }
}

int main() {
    vector<string> input = {"id", "+", "id", "*", "id"}; // Example input: id + id * id
    shiftReduceParser(input);
    return 0;
}
