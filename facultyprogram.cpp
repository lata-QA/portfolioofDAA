#include <iostream>
#include <unordered_map>
#include <queue>
#include <string>
#include <memory>
using namespace std;

// Faculty structure
struct Faculty {
    int id;
    string name;
    string qualification;
    int experience; // years of experience
    Faculty(int i, string n, string q, int e) : id(i), name(n), qualification(q), experience(e) {}
};

// Comparator for priority queue (higher priority for more experience)
struct CompareFaculty {
    bool operator()(const shared_ptr<Faculty>& f1, const shared_ptr<Faculty>& f2) {
        return f1->experience < f2->experience; // Higher experience gets higher priority
    }
};

// AVL Tree Node
struct AVLNode {
    shared_ptr<Faculty> faculty;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(shared_ptr<Faculty> f) : faculty(f), left(nullptr), right(nullptr), height(1) {}
};

// AVL Tree class
class AVLTree {
private:
    AVLNode* root;

    int getHeight(AVLNode* node) {
        return node ? node->height : 0;
    }

    int getBalanceFactor(AVLNode* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        return x;
    }

    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        return y;
    }

    AVLNode* insert(AVLNode* node, shared_ptr<Faculty> faculty) {
        if (!node) return new AVLNode(faculty);
        if (faculty->id < node->faculty->id)
            node->left = insert(node->left, faculty);
        else if (faculty->id > node->faculty->id)
            node->right = insert(node->right, faculty);
        else
            return node;
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        int balance = getBalanceFactor(node);
        if (balance > 1 && faculty->id < node->left->faculty->id)
            return rotateRight(node);
        if (balance < -1 && faculty->id > node->right->faculty->id)
            return rotateLeft(node);
        if (balance > 1 && faculty->id > node->left->faculty->id) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && faculty->id < node->right->faculty->id) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }

    void inOrder(AVLNode* node) {
        if (!node) return;
        inOrder(node->left);
        cout << "ID: " << node->faculty->id << ", Name: " << node->faculty->name << endl;
        inOrder(node->right);
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(shared_ptr<Faculty> faculty) {
        root = insert(root, faculty);
    }

    void display() {
        inOrder(root);
    }
};

int main() {
    unordered_map<int, shared_ptr<Faculty>> facultyTable; // Hash table for faculty
    priority_queue<shared_ptr<Faculty>, vector<shared_ptr<Faculty>>, CompareFaculty> recruitmentQueue; // Priority queue
    AVLTree facultyTree; // AVL Tree for sorting and searching

    while (true) {
        cout << "\n1. Add Faculty\n2. Display All Faculty (Sorted by ID)\n3. Display Recruitment Priority\n4. Exit\nEnter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            int id, experience;
            string name, qualification;
            cout << "Enter Faculty ID: ";
            cin >> id;
            cout << "Enter Faculty Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Qualification: ";
            getline(cin, qualification);
            cout << "Enter Experience (years): ";
            cin >> experience;

            auto faculty = make_shared<Faculty>(id, name, qualification, experience);

            // Add to hash table
            facultyTable[id] = faculty;

            // Add to priority queue
            recruitmentQueue.push(faculty);

            // Add to AVL tree
            facultyTree.insert(faculty);

            cout << "Faculty added successfully!\n";
        } else if (choice == 2) {
            cout << "\nFaculty sorted by ID (AVL Tree):\n";
            facultyTree.display();
        } else if (choice == 3) {
            cout << "\nRecruitment priority:\n";
            auto tempQueue = recruitmentQueue; // Copy of priority queue
            while (!tempQueue.empty()) {
                auto f = tempQueue.top();
                tempQueue.pop();
                cout << "Name: " << f->name << ", Experience: " << f->experience << " years" << endl;
            }
        } else if (choice == 4) {
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
