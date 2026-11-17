#include <iostream>
using namespace std;

struct Node {
    int id;
    int weight;
    Node* next;
};

int numTrucks;
int truckQueue[50];    // queue of truck capacities
int assignTruck[100];  // assignTruck[i] = truck number of the i-th container

bool solve(Node* cur, int idx, int truck, int remaining, int remainingWeight) {
    // Base case: all containers are placed
    if (cur == NULL)
        return true;

    // Pruning rule 2: remaining weight is more than the total remaining capacity
    int totalCapacity = remaining;
    for (int i = truck + 1; i < numTrucks; i++)
        totalCapacity += truckQueue[i];

    if (remainingWeight > totalCapacity)
        return false;

    // Choice 1: load this container on the current truck
    // Pruning rule 1: only if it fits
    if (cur->weight <= remaining) {
        assignTruck[idx] = truck;                 // choose
        if (solve(cur->next, idx + 1, truck, remaining - cur->weight, remainingWeight - cur->weight))
            return true;                          // explore
        assignTruck[idx] = -1;                    // undo (backtrack)
    }

    // Choice 2: close this truck and move to the next truck
    if (truck + 1 < numTrucks)
        return solve(cur, idx, truck + 1, truckQueue[truck + 1], remainingWeight);

    return false;
}

int main() {
    int n;
    cout << "Enter number of containers: ";
    cin >> n;

    // Build the linked list
    Node* head = NULL;
    Node* tail = NULL;
    int totalWeight = 0;

    for (int i = 0; i < n; i++) {
        Node* newNode = new Node;
        cout << "Enter id and weight of container " << i + 1 << ": ";
        cin >> newNode->id >> newNode->weight;
        newNode->next = NULL;
        totalWeight += newNode->weight;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    // Build the queue of trucks
    int front = 0, rear = 0;
    cout << "Enter number of trucks: ";
    cin >> numTrucks;

    for (int i = 0; i < numTrucks; i++) {
        cout << "Enter capacity of truck " << i + 1 << ": ";
        cin >> truckQueue[rear];   // enqueue
        rear++;
    }

    // Start backtracking from the first container and the front truck
    if (solve(head, 0, front, truckQueue[front], totalWeight)) {
        for (int t = 0; t < numTrucks; t++) {
            cout << "Truck " << t + 1 << ": ";
            Node* temp = head;
            int i = 0;
            while (temp != NULL) {
                if (assignTruck[i] == t)
                    cout << temp->id << " ";
                temp = temp->next;
                i++;
            }
            cout << endl;
        }
    } else {
        cout << "Insufficient truck capacity." << endl;
    }

    return 0;
}