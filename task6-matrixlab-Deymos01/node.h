#ifndef TASK6_MATRIXLAB_DEYMOS01_NODE_H
#define TASK6_MATRIXLAB_DEYMOS01_NODE_H

#include <stack>
#include <map>
#include <fstream>

#include "matrix.h"
#include "fraction.h"
#include "errors.h"
#include "biginteger.h"

struct Node {
    virtual ~Node() {}
};

struct BracketNode : public Node {
    bool isOpenBracket;
    BracketNode(bool val) : isOpenBracket(val) {}
};

// =============================================

struct DataNode : public Node {
    virtual Matrix getValue() const = 0;
};

struct ValueNode : public DataNode {
private:
    Matrix data_;
public:
    ValueNode(const Matrix &matrix) : data_(matrix) {}
    Matrix getValue() const { return data_; }
};

namespace global {
    extern std::map<std::string, Matrix> Workspace;
}

struct ParamNode : public DataNode {
private:
    std::string name_;
public:
    ParamNode(std::string name) : name_(std::move(name)) {}
    const std::string &getName() const { return name_; }
    Matrix getValue() const {
        if (global::Workspace.find(name_) == global::Workspace.end()) {
            throw UndefinedParameter();
        }
        return global::Workspace[name_];
    }
};

struct Data {
    DataNode *parameter1 = nullptr;
    DataNode *parameter2 = nullptr;
    DataNode *parameter3 = nullptr;
    ~Data() {
        delete parameter1;
        delete parameter2;
        delete parameter3;
    }
};

// =============================================

struct ActionNode : public Node {
    virtual Node *calc(std::stack<Node *> &S) { return S.top(); }
};

struct AssignmentNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct PlusNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct MinusNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct MultiplicationNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct DivisionNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct MultElemNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct DivElemNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

//=======================================================

struct ZerosNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct OnesNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct EyeNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct LinspaceNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct VertcatNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct HorzcatNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct TransposeNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct DeterminantNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct InverseNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct SumNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct ProductNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct MinimumNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct MaximumNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct RowEchelonFormNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct RankNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct MinorNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

struct CofactorNode : public ActionNode {
    Node *calc(std::stack<Node *> &stack);
};

// ========================================================

struct DisplayNode : public ActionNode {
    std::ostream *out_;

    DisplayNode(std::ostream *out) : out_(out) {}

    Node *calc(std::stack<Node *> &stack);
};

#endif //TASK6_MATRIXLAB_DEYMOS01_NODE_H
