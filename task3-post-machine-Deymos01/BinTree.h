#ifndef TASK3_POST_MACHINE_DEYMOS01_BINTREE_H
#define TASK3_POST_MACHINE_DEYMOS01_BINTREE_H

#include "commands.h"

class BinTree {
private:
    struct Node {
        Node *left, *right, *parent;
        Command *data;

        Node(Command *x) : left(nullptr), right(nullptr), parent(nullptr), data(x) {}

        ~Node() {
            delete left;
            delete right;
            delete data;
            left = right = nullptr;
            data = nullptr;
        }

        Node *minimum() {
            Node *currNode = this;
            while (currNode->left != nullptr) {
                currNode = currNode->left;
            }
            return currNode;
        }
    };

    Node *root;
    int count;

public:
    BinTree() : root(nullptr), count(0) {}

    ~BinTree() {
        delete root;
        root = nullptr;
        count = 0;
    }

    void insert(Command *data) {
        ++count;
        if (root == nullptr) {
            root = new Node(data);
            return;
        }
        Node *currNode = root;
        Node *parentNode;
        do {
            parentNode = currNode;
            if (data->currCommandIndex < currNode->data->currCommandIndex) {
                currNode = currNode->left;
            } else {
                currNode = currNode->right;
            }
        } while (currNode != nullptr);
        currNode = new Node(data);
        currNode->parent = parentNode;
        if (parentNode->data->currCommandIndex > data->currCommandIndex) {
            parentNode->left = currNode;
        } else {
            parentNode->right = currNode;
        }
    }

    Node *find(int index) const {
        Node *currNode = root;
        while (currNode != nullptr) {
            if (index == currNode->data->currCommandIndex) {
                break;
            }
            if (index < currNode->data->currCommandIndex) {
                currNode = currNode->left;
            } else {
                currNode = currNode->right;
            }
        }
        return currNode;
    }

    BinTree::Node *find_minimum() const {
        Node *currNode = root;
        if (currNode != nullptr) {
            currNode = currNode->minimum();
        }
        return currNode;
    }

    void balance() {
        if (root == nullptr) {
            return;
        }

        Node *currNode;
        Node *rightChild;
        Node *parentNode;
        currNode = root;
        while (currNode != nullptr) {
            if (currNode->right == nullptr) {
                currNode = currNode->left;
            } else {
                rightChild = currNode->right;
                parentNode = currNode->parent;
                if (parentNode != nullptr) { parentNode->left = rightChild; }
                else {
                    root = rightChild;
                }
                rightChild->parent = parentNode;
                currNode->right = rightChild->left;
                if (rightChild->left != nullptr) {
                    rightChild->left->parent = currNode;
                }
                rightChild->left = currNode;
                currNode->parent = rightChild;
                currNode = rightChild;
            }
        }

        int n = 0;
        int n2 = 1;
        for (int i = count + 1; i > 1; ++n, n2 <<= 1, i >>= 1) {}

        int excess = count - (n2 - 1);

        Node *red;
        Node *black;
        red = root;
        if (red == nullptr) {
            return;
        }
        while (excess > 0) {
            black = red->left;
            if (black == nullptr) {
                break;
            }
            black->right = red;
            black->parent = red->parent;
            if (red->parent != nullptr) {
                red->parent->left = black;
            } else {
                root = black;
            }
            red->parent = black;
            red->left = nullptr;
            --excess;
            red = black->left;
            if (red == nullptr) {
                break;
            }
        }
        int blackInStep = (n2 - 1) / 2;
        for (int step = 1; step < n; ++step, blackInStep /= 2) {
            red = root;
            if (red == nullptr || red->left == nullptr) {
                break;
            }
            for (int i = 0; i < blackInStep; ++i) {
                black = red->left;

                black->parent = red->parent;
                if (red->parent != nullptr) {
                    red->parent->left = black;
                } else {
                    root = black;
                }

                red->left = black->right;
                if (black->right != nullptr) {
                    black->right->parent = red;
                }

                black->right = red;
                red->parent = black;

                red = black->left;
                if (red == nullptr || red->left == nullptr) {
                    break;
                }
            }
        }
    }
};

#endif  // TASK3_POST_MACHINE_DEYMOS01_BINTREE_H