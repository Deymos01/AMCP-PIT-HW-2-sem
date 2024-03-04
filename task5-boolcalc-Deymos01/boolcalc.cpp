#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>

#include "boolexpr.h"
#include "errors.h"

void printInfo();
void parseFile(std::ifstream & /*inFile*/, std::vector<BooleanExpression *> & /*expressions*/);

int main(int argc, char *argv[]) {
    if (argc < 2 || argc == 3) {
        throw std::invalid_argument("Not enough arguments.");
    }
    if (argc > 4) {
        throw std::invalid_argument("Too much arguments.");
    }

    if (argc == 2) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "?") == 0) {
            printInfo();
        } else {
            throw std::invalid_argument("Invalid arguments.");
        }
    }

    if (argc == 4) {
        std::ifstream inFile(argv[argc - 2]);
        std::ofstream outFile(argv[argc - 1]);

        if (!inFile) {
            outFile.close();
            std::string errorInfo = "Failed to open the file: " + std::string(argv[argc - 2]);
            throw std::ifstream::failure(errorInfo);
        }
        if (!outFile) {
            inFile.close();
            std::string errorInfo = "Failed to open the file: " + std::string(argv[argc - 1]);
            throw std::ifstream::failure(errorInfo);
        }

        std::string command = argv[1];
        if (command != "-cnf" && command != "-dnf" && command != "-zh" && command != "-isfull") {
            inFile.close();
            outFile.close();
            throw std::invalid_argument("Unknown command: " + command);
        }

        std::vector<BooleanExpression *> expressions;
        try {
            parseFile(inFile, expressions);
        } catch (const Error &error) {
            outFile << "error";

            for (size_t i = 0; i < expressions.size(); ++i) {
                delete expressions[i];
            }

            inFile.close();
            outFile.close();
            std::cout << error.what() << std::endl;
        }

        if (command == "-cnf") {
            // Build the conjunctive normal form
            for (int i = 0; i < expressions.size(); ++i) {
                try {
                    BooleanExpression temp = expressions[i]->cnf();
                    outFile << std::string(temp) << std::endl;
                } catch (const Error &) {
                    outFile << "cnf_error" << std::endl;
                }
            }
        } else if (command == "-dnf") {
            // Build the disjunctive normal form
            for (int i = 0; i < expressions.size(); ++i) {
                try {
                    BooleanExpression temp = expressions[i]->dnf();
                    outFile << std::string(temp) << std::endl;
                } catch (const Error &) {
                    outFile << "dnf_error" << std::endl;
                }
            }
        } else if (command == "-zh") {
            // Build the Zhegalkin's polynomial
            for (int i = 0; i < expressions.size(); ++i) {
                BooleanExpression temp = expressions[i]->zhegalkin();
                outFile << std::string(temp) << std::endl;
            }
        } else if (command == "-isfull") {
            // Check for completeness of the system of functions
            bool result = isFullSystem(expressions);
            if (!result) {
                outFile << "yes";
            } else {
                outFile << "no";
            }
        }

        for (size_t i = 0; i < expressions.size(); ++i) {
            delete expressions[i];
        }
        outFile.close();
        inFile.close();
    }
    return 0;
}

void parseFile(std::ifstream &inFile, std::vector<BooleanExpression *> &expressions) {
    char line[1024];
    BooleanExpression *expr;
    while (inFile.peek() != EOF) {
        memset(line, 0, 1024);
        inFile.getline(line, 1024, '\n');
        if (line[strlen(line) - 1] == '\r') {
            line[strlen(line) - 1] = '\0';
        }
        if (strcmp(line, "cnf_error") == 0) {
            memset(line, 0, 1024);
            line[0] = '1';
        } else if (strcmp(line, "dnf_error") == 0) {
            memset(line, 0, 1024);
            line[0] = '0';
        }
        try {
            expr = new BooleanExpression(line);
            expressions.push_back(expr);
        } catch (...) {
            throw;
        }
    }
}

void printInfo() {
    std::cout << "\n\n===========Boolean Expression Calculator===========\n"
                 "\n"
                 "Variables in the input expression: xN, where N is a natural number (e.g., x1, x2, x3, ...). They can take values of 0 and 1 (non-zero).\n"
                 "\n"
                 "Operations:\n"
                 "~     - Negation\n"
                 "&     - Conjunction\n"
                 "v     - Disjunction\n"
                 "+     - Addition modulo 2\n"
                 ">     - Implication (x → y)\n"
                 "<     - Reverse implication (x ← y)\n"
                 "=     - Equivalence\n"
                 "|     - Sheffer's stroke\n"
                 "^     - Pierce's arrow\n"
                 "\n"
                 "Usage: boolcalc [action] [input] [output]\n"
                 "\n"
                 "Actions:\n"
                 "  -cnf    Construct the Conjunctive Normal Form (CNF) for expressions in the input file and write the results to the output file.\n"
                 "  -dnf    Construct the Disjunctive Normal Form (DNF) for expressions in the input file and write the results to the output file.\n"
                 "  -zh     Construct the Zhegalkin Polynomial for expressions in the input file and write the results to the output file.\n"
                 "  -isfull Check if the function system in the input file is complete and output the result (\"yes\" or \"no\") to the output file.\n"
                 "  -h      Display this help message.\n"
                 "\n"
                 "Examples:\n"
                 "  boolcalc -cnf input.txt output.txt\n"
                 "  boolcalc -dnf input.txt output.txt\n"
                 "  boolcalc -zh input.txt output.txt\n"
                 "  boolcalc -isfull input.txt output.txt\n"
                 "  boolcalc -h" << std::endl;
}