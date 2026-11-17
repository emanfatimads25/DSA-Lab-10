#include <iostream>
using namespace std;

struct Node {
    int id;
    int cost;
    Node* left;
    Node* right;
};

int B;                  // target budget
Node* pathStack[100];   // stack for Part A (stores node pointers)
int top = 0;            // top of the Part A stack
int pathCount = 0;      // number of paths found

int ids[100];           // inorder (sorted) ids for Part B
int costs[100];         // inorder (sorted) costs for Part B
int total = 0;           // number of nodes in the sorted arrays
int subStack[100];      // stack for Part B (stores array indexes)
int subTop = 0;         // top of the Part B stack
int subCount = 0;       // number of subsets found

// Part A: go downward from one starting node and check every path
void findPaths(Node* node, int sum) {
    if (node == NULL)
        return;

    pathStack[top] = node;       // push
    top++;
    sum = sum + node->cost;

    if (sum == B) {
        pathCount++;
        cout << "Path " << pathCount << ": ";
        for (int i = 0; i < top; i++) {
            cout << pathStack[i]->id << "(" << pathStack[i]->cost << ")";
            if (i < top - 1)
                cout << " -> ";
        }
        cout << endl;
    }

    findPaths(node->left, sum);
    findPaths(node->right, sum);

    top--;                       // pop (backtrack)
}

// Part A: use every node of the tree as a starting node
void startFromEveryNode(Node* node) {
    if (node == NULL)
        return;
    findPaths(node, 0);
    startFromEveryNode(node->left);
    startFromEveryNode(node->right);
}

// Inorder traversal gives the costs in sorted (ascending) order
void inorder(Node* node) {
    if (node == NULL)
        return;
    inorder(node->left);
    ids[total] = node->id;
    costs[total] = node->cost;
    total++;
    inorder(node->right);
}

// Part B: find all subsets whose cost sum is B
void findSubsets(int start, int sum) {
    for (int i = start; i < total; i++) {
        // Pruning: this cost and all bigger costs after it will exceed B
        if (sum + costs[i] > B)
            break;

        subStack[subTop] = i;    // push
        subTop++;

        if (sum + costs[i] == B) {
            subCount++;
            cout << "Subset " << subCount << ": ";
            for (int j = 0; j < subTop; j++)
                cout << ids[subStack[j]] << "(" << costs[subStack[j]] << ") ";
            cout << endl;
        }

        findSubsets(i + 1, sum + costs[i]);

        subTop--;                // pop (backtrack)
    }
}

int main() {
    int n;
    cout << "Enter number of microservices: ";
    cin >> n;

    // Build the BST by inserting nodes one by one
    Node* root = NULL;
    for (int i = 0; i < n; i++) {
        Node* newNode = new Node;
        cout << "Enter serviceID and monthlyCost of service " << i + 1 << ": ";
        cin >> newNode->id >> newNode->cost;
        newNode->left = NULL;
        newNode->right = NULL;

        if (root == NULL) {
            root = newNode;
        } else {
            Node* cur = root;
            while (true) {
                if (newNode->cost < cur->cost) {
                    if (cur->left == NULL) {
                        cur->left = newNode;
                        break;
                    }
                    cur = cur->left;
                } else {
                    if (cur->right == NULL) {
                        cur->right = newNode;
                        break;
                    }
                    cur = cur->right;
                }
            }
        }
    }

    cout << "Enter target budget B: ";
    cin >> B;

    // Part A
    cout << endl << "Part A: Downward paths with sum " << B << endl;
    startFromEveryNode(root);
    if (pathCount == 0)
        cout << "No path found." << endl;

    // Part B
    inorder(root);
    cout << endl << "Part B: Subsets with sum " << B << endl;
    findSubsets(0, 0);
    if (subCount == 0)
        cout << "No subset found." << endl;

    return 0;
}