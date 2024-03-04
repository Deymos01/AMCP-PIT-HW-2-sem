#include <stack>
#include <map>
#include <fstream>
#include <sstream>

#include "errors.h"
#include "parsing.h"
#include "node.h"
#include "fraction.h"

void clearStack(std::stack<Node *> &S) {
    while (!S.empty()) {
        delete S.top();
        S.pop();
    }
}

void calculate(std::stack<Node *> &S) {
    std::stack<Node *> tmp;

    while (!S.empty()) {
        BracketNode *openedBracket;
        while (true) {
            openedBracket = dynamic_cast<BracketNode *>(S.top());
            if (openedBracket != nullptr && openedBracket->isOpenBracket) {
                break;
            }
            tmp.push(S.top());
            S.pop();
        }
        delete openedBracket;
        S.pop();

        if (!tmp.empty()) {
            ActionNode *action = dynamic_cast<ActionNode *>(tmp.top());
            tmp.pop();
            try {
                Node *res = action->calc(tmp);
                delete action;
                delete tmp.top();
                tmp.pop();
                if (tmp.empty()) {
                    delete res;
                } else {
                    tmp.push(res);
                }
            } catch (...) {
                throw;
            }
        }
    }
}

std::stack<Node *> process(const std::string &inpStr, std::ofstream &outFile) {
    std::map<std::string, unsigned char> commandIndex = {
            {"=",         0},
            {"+",         1},
            {"-",         2},
            {".*",        3},
            {"./",        4},
            {"*",         5},
            {"/",         6},
            {"zeros",     7},
            {"ones",      8},
            {"eye",       9},
            {"linspace",  10},
            {"vertcat",   11},
            {"horzcat",   12},
            {"transpose", 13},
            {"det",       14},
            {"inv",       15},
            {"sum",       16},
            {"prod",      17},
            {"min",       18},
            {"max",       19},
            {"disp",      20},
            {"echelon",   21},
            {"rank",      22},
            {"minor",     23},
            {"cofactor",  24},
    };

    std::stringstream str_strm(inpStr);
    std::string tmp;
    int openedBrackets = 0;
    std::stack<Node *> S;

    while (str_strm >> tmp) {
        if (tmp[0] == '(') {
            S.push(new BracketNode(true));
            ++openedBrackets;
            tmp = tmp.substr(1);
        }
        int closedBrackets = 0;
        if (tmp[tmp.size() - 1] == ')') {
            closedBrackets = static_cast<int>(tmp.size() - tmp.find(')'));
            openedBrackets -= closedBrackets;
            tmp = tmp.substr(0, tmp.find(')'));
            if (openedBrackets < 0) {
                clearStack(S);
                throw OpenedBracketError();
            }
        }

        bool flagMinus = false;
        if (tmp.size() > 1 && tmp[0] == '-') {
            S.push(new BracketNode(true));
            S.push(new MinusNode());
            tmp = tmp.substr(1);
            flagMinus = true;
        }

        if (isdigit(tmp[0]) != 0) {
            S.push(new ValueNode(Matrix(Fraction(tmp), 1, 1)));
        } else if (commandIndex.find(tmp) == commandIndex.end()) {
            S.push(new ParamNode(tmp));
        } else {
            Node *added;
            switch (commandIndex[tmp]) {
                case 0:
                    added = new AssignmentNode();
                    break;
                case 1:
                    added = new PlusNode();
                    break;
                case 2:
                    added = new MinusNode();
                    break;
                case 3:
                    added = new MultElemNode();
                    break;
                case 4:
                    added = new DivElemNode();
                    break;
                case 5:
                    added = new MultiplicationNode();
                    break;
                case 6:
                    added = new DivisionNode();
                    break;
                case 7:
                    added = new ZerosNode();
                    break;
                case 8:
                    added = new OnesNode();
                    break;
                case 9:
                    added = new EyeNode();
                    break;
                case 10:
                    added = new LinspaceNode();
                    break;
                case 11:
                    added = new VertcatNode();
                    break;
                case 12:
                    added = new HorzcatNode();
                    break;
                case 13:
                    added = new TransposeNode();
                    break;
                case 14:
                    added = new DeterminantNode();
                    break;
                case 15:
                    added = new InverseNode();
                    break;
                case 16:
                    added = new SumNode();
                    break;
                case 17:
                    added = new ProductNode();
                    break;
                case 18:
                    added = new MinimumNode();
                    break;
                case 19:
                    added = new MaximumNode();
                    break;
                case 20:
                    added = new DisplayNode(&outFile);
                    break;
                case 21:
                    added = new RowEchelonFormNode();
                    break;
                case 22:
                    added = new RankNode();
                    break;
                case 23:
                    added = new MinorNode();
                    break;
                case 24:
                    added = new CofactorNode();
                    break;
            }
            S.push(added);
        }

        if (flagMinus) {
            S.push(new BracketNode(false));
        }

        while (closedBrackets > 0) {
            S.push(new BracketNode(false));
            --closedBrackets;
        }
    }
    if (openedBrackets > 0) {
        clearStack(S);
        throw ClosedBracketError();
    }
    return S;
}