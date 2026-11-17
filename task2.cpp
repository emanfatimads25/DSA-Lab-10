#include <iostream>
using namespace std;

int target;          // target state T
int maxOps;          // maximum number of operations K
int bound;           // overflow bound M
int numOps;          // number of allowed operations
char opType[20];     // operation symbol: + - * /
int opVal[20];       // operation number
int stackArr[50];    // operation stack (stores operation index)
int top = 0;         // top of the stack
int count = 0;       // number of sequences found

void push(int x) {
    stackArr[top] = x;
    top++;
}

void pop() {
    top--;
}

void printSequence() {
    count++;
    cout << "Sequence " << count << ": ";
    if (top == 0)
        cout << "(no operations needed)";
    for (int i = 0; i < top; i++) {
        int k = stackArr[i];
        cout << opType[k] << opVal[k] << " ";
    }
    cout << endl;
}

void solve(int current, int steps) {
    // If current value equals target, this sequence is valid
    if (current == target)
        printSequence();

    // No more operations allowed
    if (steps == maxOps)
        return;

    // Try every allowed operation
    for (int i = 0; i < numOps; i++) {
        int next = current;

        if (opType[i] == '+')
            next = current + opVal[i];
        else if (opType[i] == '-')
            next = current - opVal[i];
        else if (opType[i] == '*')
            next = current * opVal[i];
        else if (opType[i] == '/') {
            if (opVal[i] == 0)
                continue;          // cannot divide by zero
            next = current / opVal[i];
        }

        // Pruning: value must not be negative or exceed the bound
        if (next < 0 || next > bound)
            continue;

        push(i);                   // choose
        solve(next, steps + 1);    // explore
        pop();                     // undo (backtrack)
    }
}

int main() {
    int start;

    cout << "Enter initial state S: ";
    cin >> start;
    cout << "Enter target state T: ";
    cin >> target;
    cout << "Enter maximum operations K: ";
    cin >> maxOps;
    cout << "Enter overflow bound M: ";
    cin >> bound;

    cout << "Enter number of allowed operations: ";
    cin >> numOps;
    for (int i = 0; i < numOps; i++) {
        cout << "Enter operation " << i + 1 << " (symbol then number, e.g. + 3): ";
        cin >> opType[i] >> opVal[i];
    }

    cout << endl;
    solve(start, 0);

    if (count == 0)
        cout << "No valid sequence found." << endl;
    else
        cout << "Total sequences: " << count << endl;

    return 0;
}