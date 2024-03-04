#ifndef TASK4_COMPRESS_NODE_H
#define TASK4_COMPRESS_NODE_H

struct Node {
    Node *left, *right;
    unsigned char symbol;
    int frequency;

    Node(unsigned char symb = '\0', int freq = 0) {
        left = right = nullptr;
        symbol = symb;
        frequency = freq;
    }

    ~Node() {
        delete left;
        delete right;
        left = nullptr;
        right = nullptr;
    }
};

#endif //TASK4_COMPRESS_NODE_H
