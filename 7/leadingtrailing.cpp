#include <bits/stdc++.h>
using namespace std;

int nt, t, top = 0;
char s[50], NT[10], T[10], st[50], l[10][10], tr[10][10];

int searchnt(char a) {
    for (int i = 0; i < nt; i++) {
        if (NT[i] == a) return i;
    }
    return -1;
}

int searchter(char a) {
    for (int i = 0; i < t; i++) {
        if (T[i] == a) return i;
    }
    return -1;
}

void push(char a) {
    s[top++] = a;
}

char pop() {
    return s[--top];
}

void installl(int a, int b) {
    if (l[a][b] == 'f') {
        l[a][b] = 't';
        push(T[b]);
        push(NT[a]);
    }
}

void installt(int a, int b) {
    if (tr[a][b] == 'f') {
        tr[a][b] = 't';
        push(T[b]);
        push(NT[a]);
    }
}

bool isOperatorPrecedenceGrammar(const vector<string>& pr) {
    for (const string& rule : pr) {
        size_t arrowPos = rule.find("->");
        if (arrowPos == string::npos) continue;

        string rhs = rule.substr(arrowPos + 2);

        // Condition 1: No production should have two consecutive non-terminals
        for (size_t i = 0; i < rhs.length() - 1; i++) {
            if (searchnt(rhs[i]) != -1 && searchnt(rhs[i + 1]) != -1) {
                cout << "❌ Not an Operator Precedence Grammar: Two consecutive non-terminals found in rule: " << rule << endl;
                return false;
            }
        }

        // Condition 2: The right-hand side must not contain ε (empty production)
        if (rhs == "ε" || rhs == "E") {
            cout << "❌ Not an Operator Precedence Grammar: ε-production found in rule: " << rule << endl;
            return false;
        }

        // Condition 3: Each production must contain at least one terminal
        bool hasTerminal = false;
        for (char ch : rhs) {
            if (searchter(ch) != -1 || (!isalpha(ch) && ch != '-' && ch != '>')) {
                hasTerminal = true;
                break;
            }
        }
        if (!hasTerminal) {
            cout << "❌ Not an Operator Precedence Grammar: No terminal found in rule: " << rule << endl;
            return false;
        }
    }
    return true;
}

int main() {
    int n;
    cout << "Enter the number of productions: ";
    cin >> n;
    cin.ignore();
    vector<string> pr(n);

    cout << "Enter the productions (format: S->aSb, S->ab, etc.):\n";
    for (int i = 0; i < n; i++) {
        getline(cin, pr[i]);
    }

    nt = 0;
    t = 0;

    // Extract Non-terminals
    for (const string& rule : pr) {
        if (searchnt(rule[0]) == -1) {
            NT[nt++] = rule[0];
        }
    }

    // Extract Terminals
    for (const string& rule : pr) {
        size_t arrowPos = rule.find("->");
        if (arrowPos == string::npos) continue;

        for (size_t j = arrowPos + 2; j < rule.length(); j++) {
            if (searchnt(rule[j]) == -1 && searchter(rule[j]) == -1) {
                T[t++] = rule[j];
            }
        }
    }

    // Initialize leading and trailing tables
    memset(l, 'f', sizeof(l));
    memset(tr, 'f', sizeof(tr));

    // Compute Leading Set
    for (int i = 0; i < nt; i++) {
        for (const string& rule : pr) {
            if (NT[searchnt(rule[0])] == NT[i]) {
                size_t arrowPos = rule.find("->");
                if (arrowPos == string::npos) continue;

                if (searchter(rule[arrowPos + 2]) != -1) {
                    installl(searchnt(rule[0]), searchter(rule[arrowPos + 2]));
                } else {
                    for (size_t k = arrowPos + 2; k < rule.length(); k++) {
                        if (searchnt(rule[k]) == -1) {
                            installl(searchnt(rule[0]), searchter(rule[k]));
                            break;
                        }
                    }
                }
            }
        }
    }

    while (top != 0) {
        char b = pop(), c = pop();
        for (const string& rule : pr) {
            if (rule[3] == b)
                installl(searchnt(rule[0]), searchter(c));
        }
    }

    // Print Leading Set
    for (int i = 0; i < nt; i++) {
        cout << "Leading(" << NT[i] << ") = { ";
        for (int j = 0; j < t; j++) {
            if (l[i][j] == 't') {
                cout << T[j] << " ";
            }
        }
        cout << "}\n";
    }

    top = 0;

    // Compute Trailing Set
    for (int i = 0; i < nt; i++) {
        for (const string& rule : pr) {
            if (NT[searchnt(rule[0])] == NT[i]) {
                size_t arrowPos = rule.find("->");
                if (arrowPos == string::npos) continue;

                if (searchter(rule.back()) != -1) {
                    installt(searchnt(rule[0]), searchter(rule.back()));
                } else {
                    for (int k = rule.length() - 1; k >= (int)arrowPos + 2; k--) {
                        if (searchnt(rule[k]) == -1) {
                            installt(searchnt(rule[0]), searchter(rule[k]));
                            break;
                        }
                    }
                }
            }
        }
    }

    while (top != 0) {
        char b = pop(), c = pop();
        for (const string& rule : pr) {
            if (rule[3] == b)
                installt(searchnt(rule[0]), searchter(c));
        }
    }

    // Print Trailing Set
    for (int i = 0; i < nt; i++) {
        cout << "Trailing(" << NT[i] << ") = { ";
        for (int j = 0; j < t; j++) {
            if (tr[i][j] == 't') {
                cout << T[j] << " ";
            }
        }
        cout << "}\n";
    }

    // Check if it's an Operator Precedence Grammar
    if (isOperatorPrecedenceGrammar(pr)) {
        cout << "The given grammar is an Operator Precedence Grammar.\n";
    } else {
        cout << " The given grammar is NOT an Operator Precedence Grammar.\n";
    }

    return 0;
}

