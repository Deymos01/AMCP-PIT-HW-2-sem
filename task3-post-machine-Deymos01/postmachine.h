#ifndef TASK3_POST_MACHINE_DEYMOS01_POSTMACHINE_H
#define TASK3_POST_MACHINE_DEYMOS01_POSTMACHINE_H

#include <fstream>

#include "BinTree.h"
#include "errors.h"

class PostMachine {
private:
    struct Tape {
        int position;
        int len;
        char *tape;

        ~Tape() {
            delete[] tape;
            tape = nullptr;
        }
    };

    int length_;
    const BinTree *program;
public:
    PostMachine(const int &length, const BinTree *inputProgram) : length_(length), program(inputProgram) {}

    std::string calc(std::string const &, int);
};

void loadProgram(std::ifstream &, BinTree *);

#endif  // TASK3_POST_MACHINE_DEYMOS01_POSTMACHINE_H