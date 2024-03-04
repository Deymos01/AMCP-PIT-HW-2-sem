#include <cstring>
#include <stack>
#include <algorithm>

#include "boolexpr.h"
#include "actions.h"
#include "errors.h"

BooleanExpression::BooleanExpression(const char *str) {
    // Process infix form
    size_t len = strlen(str);
    len = (len < 10) ? 20 : len * 2;
    char *str_infix = new char[len];
    char *str_postfix = new char[len];

    try {
        InfixFilter(str, str_infix);
        InfixToPostfix(str_infix, str_postfix);
        root = PostfixToTree(str_postfix);
    } catch (...) {
        delete[] str_infix;
        delete[] str_postfix;
        root = nullptr;
        throw;
    }

    fillFuncResults();

    delete[] str_infix;
    delete[] str_postfix;
}

BooleanExpression BooleanExpression::cnf() {
    // Build the conjunctive normal form
    std::string result;
    int conjCount = 0;

    for (int i = 0; i < funcResults.size(); ++i) {
        if (!funcResults[i]) {
            ++conjCount;
            if (!result.empty()) {
                result += " & ";
            }

            std::string conj = "(";
            for (int j = (int) (variables.size()) - 1; j >= 0; --j) {
                if ((i & 1 << j) != 0) {
                    conj += "~";
                }
                conj += variables[variables.size() - j - 1]->str();
                if (j != 0) {
                    conj += "v";
                }
            }
            result += conj + ")";
        }
    }

    if (funcResults.size() == 1) {
        if (conjCount == 1) {
            result = "0";
        } else {
            throw ErrorCNF();
        }
    }

    BooleanExpression tmp(result.c_str());
    return tmp;
}

BooleanExpression BooleanExpression::dnf() {
    // Build the disjunctive normal form
    std::string result;
    int disjCounter = 0;

    for (int i = 0; i < funcResults.size(); ++i) {
        if (funcResults[i]) {
            ++disjCounter;
            if (!result.empty()) {
                result += " v ";
            }

            std::string disj;
            for (int j = (int) (variables.size()) - 1; j >= 0; --j) {
                if ((i & 1 << j) == 0) {
                    disj += "~";
                }
                disj += variables[variables.size() - j - 1]->str();
                if (j != 0) {
                    disj += "&";
                }
            }
            result += disj;
        }
    }

    if (funcResults.size() == 1) {
        if (disjCounter == 1) {
            result = "1";
        } else {
            throw ErrorDNF();
        }
    }

    BooleanExpression temp(result.c_str());
    return temp;
}

BooleanExpression BooleanExpression::zhegalkin() noexcept {
    // Build the Zhegalkin's polynomial
    std::string result;
    std::vector<bool> boolVals(funcResults);
    unsigned int counter = 0;

    for (int i = 0; i < funcResults.size(); ++i) {
        if (i != 0) {
            for (int j = 0; j < boolVals.size() - 1; ++j) {
                boolVals[j] = boolVals[j] != boolVals[j + 1];
            }
            boolVals.erase(boolVals.end() - 1);
        }

        if (boolVals[0]) {
            std::string summand;
            if (counter != 0) {
                summand += " + ";
            }
            if (i == 0) {
                summand = "1";
            } else {
                unsigned int varCounter = 0;
                for (int j = (int) (variables.size()) - 1; j >= 0; --j) {
                    if ((i & 1 << j) != 0) {
                        if (varCounter != 0) {
                            summand += "&";
                        }
                        summand += "x" + std::to_string(variables.size() - j);
                        ++varCounter;
                    }
                }
            }
            result += summand;
            ++counter;
        }
    }

    if (funcResults.size() == 1 && !funcResults[0]) {
        result = "0";
    }

    BooleanExpression temp(result.c_str());
    return temp;
}

void BooleanExpression::InfixFilter(const char *inStr, char *outStr) {
    int indexInStr = 0;
    int indexOutStr = 0;
    char ch;

    std::string validChar = "xv~&+<>=|^()";
    while ((ch = inStr[indexInStr++]) != 0) {
        // skip spaces and tabs
        if (ch == ' ' || ch == '\t') {
            continue;
        }

        if (ch == 'x') {
            outStr[indexOutStr++] = ch;
            int counter = 0;
            while (isdigit(inStr[indexInStr]) != 0) {
                outStr[indexOutStr++] = inStr[indexInStr++];
                ++counter;
            }
            if (counter == 0) {
                throw ErrorParameter("x");
            }
        } else if (ch == '0') {
            outStr[indexOutStr++] = 'f';
        } else if (ch == '1') {
            outStr[indexOutStr++] = 't';
        } else if (validChar.find(ch) != -1) {
            outStr[indexOutStr++] = ch;
        } else {
            const char str[2]{ch, '\0'};
            throw ErrorSymbol(str);
        }
    }
    outStr[indexOutStr] = '\0';
}

void BooleanExpression::InfixToPostfix(const char *inStr, char *outStr) {
    int indexInStr = 0;
    int indexOutStr = 0;
    int row;
    int col;
    unsigned char action;
    std::stack<char> S;

    do {
        col = actionColNumber(inStr[indexInStr]);
        row = S.empty() ? 0 : actionRowNumber(S.top());
        action = ActionsTable[row][col];
        /*
         * 1. Write from input to output with index
         * 2. Push to stack
         * 3. Write from stack to output
         * 4. Write from input to output
         * 5. Delete top symbol in stack and go to next symbol in input
         * 6. Error. There is no ')'
         * 7. Error. There is no '('
         * 8. OK. End of converting
         */
        switch (action) {
            case 1:
                outStr[indexOutStr++] = inStr[indexInStr++];
                while (isdigit(inStr[indexInStr]) != 0) {
                    outStr[indexOutStr++] = inStr[indexInStr++];
                }
                break;
            case 2:
                S.push(inStr[indexInStr++]);
                break;
            case 3:
                outStr[indexOutStr++] = S.top();
                S.pop();
                break;
            case 4:
                outStr[indexOutStr++] = inStr[indexInStr++];
                break;
            case 5:
                S.pop();
                ++indexInStr;
                break;
            case 6:
                throw ErrorBracketsClose(inStr);
            case 7:
                throw ErrorBracketsOpen(inStr);
            case 8:
                outStr[indexOutStr] = '\0';
                break;
        }
    } while (action != 8);
}

BoolNode *BooleanExpression::PostfixToTree(const char *str) {
    int indexInStr = 0;
    std::stack<BoolNode *> S;
    unsigned char ch;
    BoolNode *result;
    BoolNode *left;
    BoolNode *right;
    try {
        while ((ch = str[indexInStr]) != 0) {
            left = right = nullptr;
            switch (ch) {
                case '~':
                    if (S.empty()) { throw 1; }
                    left = S.top();
                    S.pop();
                    result = new NegationNode(left);
                    break;
                case '&':
                    if (S.size() < 2) { throw 1; }
                    right = S.top();
                    S.pop();
                    left = S.top();
                    S.pop();
                    result = new ConjunctionNode(left, right);
                    break;
                case 'v':
                    if (S.size() < 2) { throw 1; }
                    right = S.top();
                    S.pop();
                    left = S.top();
                    S.pop();
                    result = new DisjunctionNode(left, right);
                    break;
                case '+':
                    if (S.size() < 2) { throw 1; }
                    right = S.top();
                    S.pop();
                    left = S.top();
                    S.pop();
                    result = new ExclDisjunctionNode(left, right);
                    break;
                case '>':
                    if (S.size() < 2) { throw 1; }
                    right = S.top();
                    S.pop();
                    left = S.top();
                    S.pop();
                    result = new ImplicationNode(left, right);
                    break;
                case '<':
                    if (S.size() < 2) { throw 1; }
                    right = S.top();
                    S.pop();
                    left = S.top();
                    S.pop();
                    result = new ReverseImplicationNode(left, right);
                    break;
                case '=':
                    if (S.size() < 2) { throw 1; }
                    right = S.top();
                    S.pop();
                    left = S.top();
                    S.pop();
                    result = new EquivalenceNode(left, right);
                    break;
                case '|':
                    if (S.size() < 2) { throw 1; }
                    right = S.top();
                    S.pop();
                    left = S.top();
                    S.pop();
                    result = new ShefferNode(left, right);
                    break;
                case '^':
                    if (S.size() < 2) { throw 1; }
                    right = S.top();
                    S.pop();
                    left = S.top();
                    S.pop();
                    result = new PierceNode(left, right);
                    break;
                default:
                    if (ch == 'x') {
                        size_t id = 0;
                        while (isdigit(ch = str[++indexInStr]) != 0) {
                            id = id * 10 + (ch - '0');
                        }

                        bool existFlag = false;
                        int i = 0;
                        for (; i < variables.size(); ++i) {
                            if (id == variables[i]->getId()) {
                                existFlag = true;
                                break;
                            }
                        }

                        if (existFlag) {
                            result = variables[i];
                        } else {
                            result = new ParamNode(id);
                            variables.push_back(dynamic_cast<ParamNode *>(result));
                        }
                        --indexInStr;
                    } else if (ch == 't') {
                        result = new NumNode(true);
                    } else if (ch == 'f') {
                        result = new NumNode(false);
                    } else {
                        const char s[2]{static_cast<char>(ch), '\0'};
                        throw ErrorSymbol(s);
                    }
            }
            S.push(result);
            ++indexInStr;
        }
    } catch (...) {
        delete result;
        delete left;
        delete right;
        while (!S.empty()) {
            left = S.top();
            S.pop();
            delete left;
        }
        throw;
    }
    if (S.size() > 1) {
        left = right = nullptr;
        throw ErrorNoOperand();
    }
    result = S.top();
    S.pop();
    return result;
}

BooleanExpression::operator std::string() const {
    return root->str();
}

void BooleanExpression::fillFuncResults() noexcept {
    // bruteforce all values
    for (size_t i = 0; i < 1 << variables.size(); ++i) {
        for (int bit = 0; bit < variables.size(); ++bit) {
            if ((i & (1 << bit)) != 0) {
                variables[variables.size() - bit - 1]->setValue(true);
            } else {
                variables[variables.size() - bit - 1]->setValue(false);
            }
        }
        bool answ = root->calc();
        funcResults.push_back(answ);
    }
}

// ========================Classes of functions=========================

bool BooleanExpression::belongClassT0() noexcept {
    return !funcResults[0];
}
bool BooleanExpression::belongClassT1() noexcept {
    return funcResults[funcResults.size() - 1];
}
bool BooleanExpression::belongClassS() noexcept {
    for (int i = 0; i < funcResults.size() / 2; ++i) {
        if (funcResults[i] == funcResults[funcResults.size() - i - 1]) {
            return false;
        }
    }
    return true;
}
bool BooleanExpression::belongClassM() noexcept {
    unsigned int step = funcResults.size() / 2;
    for (; step > 0; step >>= 1) {
        // На каждой итерации внешнего цикла бежим константное количество раз по вектор-функции
        unsigned int a = 0;
        unsigned int b = step;
        for (int j = 0; j < funcResults.size() / 2; ++j, ++a, ++b) {
            if ((int) (funcResults[a]) > (int) (funcResults[b])) {
                return false;
            }
            if ((a + 1) % step == 0) {
                a += step;
                b += step;
            }
        }
    }
    return true;
}
bool BooleanExpression::belongClassL() noexcept {
    std::vector<bool> boolVals(funcResults);

    for (int i = 0; i < funcResults.size(); ++i) {
        if (i != 0) {
            for (int j = 0; j < boolVals.size() - 1; ++j) {
                boolVals[j] = boolVals[j] != boolVals[j + 1];
            }
            boolVals.erase(boolVals.end() - 1);
        }

        if (boolVals[0] && i != 0) {
            if ((i & (i - 1)) != 0) {
                return false;
            }
        }
    }
    return true;
}

bool isFullSystem(const std::vector<BooleanExpression *> &vec) noexcept {
    // Check for completeness of the system of functions
    bool T0 = true;
    bool T1 = true;
    bool L = true;
    bool M = true;
    bool S = true;

    for (auto *elem: vec) {
        if (T0 && !elem->belongClassT0()) {
            T0 = false;
        }
        if (T1 && !elem->belongClassT1()) {
            T1 = false;
        }
        if (L && !elem->belongClassL()) {
            L = false;
        }
        if (M && !elem->belongClassM()) {
            M = false;
        }
        if (S && !elem->belongClassS()) {
            S = false;
        }
    }
    return T0 || T1 || L || M || S;
}