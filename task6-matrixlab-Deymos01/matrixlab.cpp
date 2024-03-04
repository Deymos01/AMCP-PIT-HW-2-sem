#include <fstream>
#include <iostream>
#include <string>
#include <stack>

#include "parsing.h"
#include "errors.h"


int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Must be 2 arguments: name input file and output file" << std::endl;
        return -1;
    }
    std::ifstream inFile(argv[1]);
    if (!inFile) {
        std::cerr << "Can't open file: " << argv[1] << std::endl;
        return -2;
    }
    std::ofstream outFile(argv[2]);
    if (!outFile) {
        inFile.close();
        std::cerr << "Can't open file: " << argv[2] << std::endl;
        return -2;
    }

    std::string inStr;
    int countLine = 1;
    std::stack<Node *> S;
    while (inFile.peek() != EOF) {
        std::getline(inFile, inStr);
        try {
            S = process(inStr, outFile);
            calculate(S);
        } catch (const Error &error) {
            clearStack(S);
            inFile.close();
            outFile.clear();
            std::cerr << error.what() << std::endl;
            outFile << "Error in line " << countLine;
            outFile.close();
            return 0;
        }
        ++countLine;
    }

    outFile.close();
    inFile.close();
    return 0;
}
