#ifndef TASK6_MATRIXLAB_DEYMOS01_PARSING_H
#define TASK6_MATRIXLAB_DEYMOS01_PARSING_H

#include <string>
#include <stack>
#include <fstream>
#include "node.h"

void clearStack(std::stack<Node *> &stack);
void calculate(std::stack<Node *> &stack);
std::stack<Node *> process(const std::string &inpString, std::ofstream &outFile);

#endif //TASK6_MATRIXLAB_DEYMOS01_PARSING_H
