#include <iostream>
#include <cstring>
using namespace std;

char prod[20][20], listofvar[26] = "ABCDEFGHIJKLMNOPQR";
int novar = 1, i = 0, j = 0, k = 0, n = 0, m = 0, arr[30];
int noitem = 0; // number of item sets

// Structure to hold a grammar production
struct Grammar {
    char lhs;
    char rhs[8];
} g[20], item[20], clos[20][10];

// New structure to store transitions (GOTO moves)
struct Transition {
    int from;
    char symbol;
    int to;
} trans[100];
int transCount = 0;

// Checks if 'variable' is a grammar variable (nonterminal) and returns an index (or 0 if not found)
int isvariable(char variable) {
    for (int i = 0; i < novar; i++)
        if (g[i].lhs == variable)
            return i + 1;
    return 0;
}

/*
  The modified findclosure function:
  - It builds the candidate item set (stored in clos[newSetIndex]) corresponding to a move on symbol 'a' from state 'z'
  - It then computes the closure.
  - Next, it checks if this candidate set is already present.
  - Whether new or existing, it records a transition: from state z on symbol a to the appropriate state.
*/
void findclosure(int z, char a) {
    int newSetIndex = noitem; // candidate index for new state
    int nItems = 0; // number of items in the candidate set
    int i, j, k, l;

    // For each item in state z, if dot is before symbol 'a', move dot right
    for (i = 0; i < arr[z]; i++) {
        int len = strlen(clos[z][i].rhs);
        for (j = 0; j < len; j++) {
            if (clos[z][i].rhs[j] == '.' && clos[z][i].rhs[j + 1] == a) {
                clos[newSetIndex][nItems].lhs = clos[z][i].lhs;
                strcpy(clos[newSetIndex][nItems].rhs, clos[z][i].rhs);
                // swap the dot with the symbol 'a'
                char temp = clos[newSetIndex][nItems].rhs[j];
                clos[newSetIndex][nItems].rhs[j] = clos[newSetIndex][nItems].rhs[j + 1];
                clos[newSetIndex][nItems].rhs[j + 1] = temp;
                nItems++;
            }
        }
    }

    // Compute the closure of the candidate set:
    for (i = 0; i < nItems; i++) {
        int len = strlen(clos[newSetIndex][i].rhs);
        for (j = 0; j < len; j++) {
            if (clos[newSetIndex][i].rhs[j] == '.' && isvariable(clos[newSetIndex][i].rhs[j + 1]) > 0) {
                for (k = 0; k < novar; k++) {
                    if (clos[newSetIndex][i].rhs[j + 1] == clos[0][k].lhs) {
                        // Check if the item is already in the candidate set
                        for (l = 0; l < nItems; l++)
                            if (clos[newSetIndex][l].lhs == clos[0][k].lhs &&
                                strcmp(clos[newSetIndex][l].rhs, clos[0][k].rhs) == 0)
                                break;
                        if (l == nItems) {
                            clos[newSetIndex][nItems].lhs = clos[0][k].lhs;
                            strcpy(clos[newSetIndex][nItems].rhs, clos[0][k].rhs);
                            nItems++;
                        }
                    }
                }
            }
        }
    }
    arr[newSetIndex] = nItems;

    // Check if the candidate set already exists in our collection.
    int foundIndex = -1;
    for (i = 0; i < noitem; i++) {
        if (arr[i] == nItems) {
            bool same = true;
            for (j = 0; j < nItems; j++) {
                if (clos[newSetIndex][j].lhs != clos[i][j].lhs ||
                    strcmp(clos[newSetIndex][j].rhs, clos[i][j].rhs) != 0) {
                    same = false;
                    break;
                }
            }
            if (same) {
                foundIndex = i;
                break;
            }
        }
    }

    int transitionState;
    if (foundIndex != -1) {
        // Already exists; the transition goes to the found state.
        transitionState = foundIndex;
    } else {
        // This is a new state; record it and update noitem.
        transitionState = newSetIndex;
        noitem++;
    }

    // Record the transition from state z on symbol a.
    trans[transCount].from = z;
    trans[transCount].symbol = a;
    trans[transCount].to = transitionState;
    transCount++;
}

int main() {
    cout << "ENTER THE PRODUCTIONS OF THE GRAMMAR (enter 0 to end) :\n";
    do {
        cin >> prod[i++];
    } while (strcmp(prod[i - 1], "0") != 0);

    // Process productions into grammar structure.
    for (n = 0; n < i - 1; n++) {
        m = 0;
        j = novar;
        g[novar++].lhs = prod[n][0];
        for (k = 3; k < (int)strlen(prod[n]); k++) {
            if (prod[n][k] != '|')
                g[j].rhs[m++] = prod[n][k];
            if (prod[n][k] == '|') {
                g[j].rhs[m] = '\0';
                m = 0;
                j = novar;
                g[novar++].lhs = prod[n][0];
            }
        }
    }
    // Find a new symbol not used in grammar variables to create the augmented production.
    for (i = 0; i < 26; i++)
        if (!isvariable(listofvar[i]))
            break;
    g[0].lhs = listofvar[i];
    char temp[2] = { g[1].lhs, '\0' };
    strcat(g[0].rhs, temp);

    cout << "\n\nAugmented Grammar:\n";
    for (i = 0; i < novar; i++)
        cout << "\n" << g[i].lhs << " -> " << g[i].rhs << " ";

    // Create the initial item set (I0).
    // For each production, add the dot at the beginning (unless production is epsilon).
    for (i = 0; i < novar; i++) {
        clos[noitem][i].lhs = g[i].lhs;
        strcpy(clos[noitem][i].rhs, g[i].rhs);
        if (strcmp(clos[noitem][i].rhs, "ε") == 0)
            strcpy(clos[noitem][i].rhs, ".");
        else {
            int len = strlen(clos[noitem][i].rhs);
            // Shift the string one position to the right.
            for (int j = len + 1; j >= 0; j--)
                clos[noitem][i].rhs[j] = clos[noitem][i].rhs[j - 1];
            clos[noitem][i].rhs[0] = '.';
        }
    }
    arr[noitem++] = novar;

    // Compute GOTO transitions for each state.
    for (int z = 0; z < noitem; z++) {
        char list[10];
        int l = 0;
        // Collect all symbols immediately following a dot in items of state z.
        for (j = 0; j < arr[z]; j++) {
            int len = strlen(clos[z][j].rhs);
            for (k = 0; k < len - 1; k++) {
                if (clos[z][j].rhs[k] == '.') {
                    // Avoid duplicates.
                    int m;
                    for (m = 0; m < l; m++)
                        if (list[m] == clos[z][j].rhs[k + 1])
                            break;
                    if (m == l)
                        list[l++] = clos[z][j].rhs[k + 1];
                }
            }
        }
        // For each symbol, compute the GOTO (closure) from state z.
        for (int x = 0; x < l; x++)
            findclosure(z, list[x]);
    }

    // Print the set of item sets.
    cout << "\n\nTHE SET OF ITEMS ARE:\n";
    for (int z = 0; z < noitem; z++) {
        cout << "\nI" << z << "\n";
        for (j = 0; j < arr[z]; j++)
            cout << clos[z][j].lhs << " -> " << clos[z][j].rhs << "\n";
    }

    // Print the recorded transitions.
    cout << "\nTRANSITIONS:\n";
    for (int i = 0; i < transCount; i++) {
        cout << "GOTO(I" << trans[i].from << ", " << trans[i].symbol << ") : I" << trans[i].to << "\n";
    }

    return 0;
}
