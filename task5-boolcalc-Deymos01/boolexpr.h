#ifndef BOOLEXPR_H
#define BOOLEXPR_H

#include <vector>
#include <fstream>

#include "boolnode.h"

class BooleanExpression {
private:
    std::vector<ParamNode *> variables;
    std::vector<bool> funcResults;
    BoolNode *root;
    BoolNode *PostfixToTree(const char *);
    static void InfixFilter(const char *, char *);
    static void InfixToPostfix(const char *, char *);
    void fillFuncResults() noexcept;
    void sortVars() noexcept;
public:
    BooleanExpression(const char *);
    ~BooleanExpression() {
        delete root;
        root = nullptr;
    }

    BooleanExpression cnf();  // Build the conjunctive normal form
    BooleanExpression dnf();  // Build the disjunctive normal form
    BooleanExpression zhegalkin() noexcept;  // Build the Zhegalkin's polynomial

    // ======================Classes of functions==========================
    bool belongClassT0() noexcept;
    bool belongClassT1() noexcept;
    bool belongClassS() noexcept;
    bool belongClassM() noexcept;
    bool belongClassL() noexcept;

    operator std::string() const;
};

bool isFullSystem(const std::vector<BooleanExpression *> &) noexcept;  // Check for completeness of the system of functions

#endif  // BOOLEXPR_H