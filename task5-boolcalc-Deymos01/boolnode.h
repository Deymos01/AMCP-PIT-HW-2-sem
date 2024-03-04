#ifndef TASK5_BOOLCALC_DEYMOS01_BOOLNODE_H
#define TASK5_BOOLCALC_DEYMOS01_BOOLNODE_H

#include <string>
#include <set>

class BoolNode {
public:
    virtual bool calc() const = 0;
    virtual std::string str() const = 0;
    virtual int getWeight() const = 0;
    virtual ~BoolNode() {}
};

// ---------------------------------------------------

class NumNode : public BoolNode {
private:
    const bool val;
    const int weight = 9;
public:
    NumNode(bool x) : val(x) {}
    bool calc() const { return val; }
    std::string str() const {
        return std::to_string(static_cast<int>(val));
    }
    int getWeight() const { return weight; }
};

class ParamNode : public BoolNode {
private:
    const size_t id;
    bool val;
    const int weight = 9;
public:
    ParamNode(size_t x) : id(x), val(false) {}
    bool calc() const { return val; }
    std::string str() const { return "x" + std::to_string(id); }

    void setValue(bool x) { val = x; }
    size_t getId() const { return id; }
    int getWeight() const { return weight; }
};

bool operator==(const ParamNode&, const ParamNode&);
bool operator!=(const ParamNode&, const ParamNode&);
bool operator<(const ParamNode&, const ParamNode&);
bool operator<=(const ParamNode&, const ParamNode&);
bool operator>(const ParamNode&, const ParamNode&);
bool operator>=(const ParamNode&, const ParamNode&);

// ==================================Binary Nodes==============================================

namespace global {
    extern std::set<BoolNode *> deletedPointers;
}
class BinNode : public BoolNode {
protected:
    BoolNode *left, *right;
public:
    BinNode(BoolNode *L, BoolNode *R) : left(L), right(R) {}
    ~BinNode() {
        if (global::deletedPointers.find(left) == global::deletedPointers.end()) {
            delete left;
            global::deletedPointers.insert(left);
        }
        if (global::deletedPointers.find(right) == global::deletedPointers.end()) {
            delete right;
            global::deletedPointers.insert(right);
        }
    }
};

class ConjunctionNode : public BinNode {
private:
    const int weight = 7;
public:
    ConjunctionNode(BoolNode *L, BoolNode *R) : BinNode(L, R) {}
    bool calc() const { return left->calc() && right->calc(); }
    std::string str() const {
        if (left->getWeight() < weight && right->getWeight() < weight) {
            return "(" + left->str() + ")" + " & " + "(" + right->str() + ")";
        }
        if (left->getWeight() < weight && right->getWeight() >= weight) {
            return "(" + left->str() + ")" + " & " + right->str();
        }
        if (left->getWeight() >= weight && right->getWeight() < weight) {
            return left->str() + " & " + "(" + right->str() + ")";
        }
        return left->str() + " & " + right->str();
    }
    int getWeight() const { return weight; }
};

class DisjunctionNode : public BinNode {
private:
    const int weight = 6;
public:
    DisjunctionNode(BoolNode *L, BoolNode *R) : BinNode(L, R) {}
    bool calc() const { return left->calc() || right->calc(); }
    std::string str() const {
        if (left->getWeight() < weight && right->getWeight() < weight) {
            return "(" + left->str() + ")" + " v " + "(" + right->str() + ")";
        }
        if (left->getWeight() < weight && right->getWeight() >= weight) {
            return "(" + left->str() + ")" + " v " + right->str();
        }
        if (left->getWeight() >= weight && right->getWeight() < weight) {
            return left->str() + " v " + "(" + right->str() + ")";
        }
        return left->str() + " v " + right->str();
    }
    int getWeight() const { return weight; }
};

class ExclDisjunctionNode : public BinNode {
private:
    const int weight = 5;
public:
    ExclDisjunctionNode(BoolNode *L, BoolNode *R) : BinNode(L, R) {}
    bool calc() const { return left->calc() != right->calc(); }
    std::string str() const {
        if (left->getWeight() < weight && right->getWeight() < weight) {
            return "(" + left->str() + ")" + " + " + "(" + right->str() + ")";
        }
        if (left->getWeight() < weight && right->getWeight() >= weight) {
            return "(" + left->str() + ")" + " + " + right->str();
        }
        if (left->getWeight() >= weight && right->getWeight() < weight) {
            return left->str() + " + " + "(" + right->str() + ")";
        }
        return left->str() + " + " + right->str();
    }
    int getWeight() const { return weight; }
};

class ImplicationNode : public BinNode {
private:
    const int weight = 2;
public:
    ImplicationNode(BoolNode *L, BoolNode *R) : BinNode(L, R) {}
    bool calc() const { return static_cast<int>(left->calc()) <= static_cast<int>(right->calc()); }
    std::string str() const {
        if (left->getWeight() < weight && right->getWeight() < weight) {
            return "(" + left->str() + ")" + " > " + "(" + right->str() + ")";
        }
        if (left->getWeight() < weight && right->getWeight() >= weight) {
            return "(" + left->str() + ")" + " > " + right->str();
        }
        if (left->getWeight() >= weight && right->getWeight() < weight) {
            return left->str() + " > " + "(" + right->str() + ")";
        }
        return left->str() + " > " + right->str();
    }
    int getWeight() const { return weight; }
};

class ReverseImplicationNode : public BinNode {
private:
    const int weight = 2;
public:
    ReverseImplicationNode(BoolNode *L, BoolNode *R) : BinNode(L, R) {}
    bool calc() const { return static_cast<int>(left->calc()) >= static_cast<int>(right->calc()); }
    std::string str() const {
        if (left->getWeight() < weight && right->getWeight() < weight) {
            return "(" + left->str() + ")" + " < " + "(" + right->str() + ")";
        }
        if (left->getWeight() < weight && right->getWeight() >= weight) {
            return "(" + left->str() + ")" + " < " + right->str();
        }
        if (left->getWeight() >= weight && right->getWeight() < weight) {
            return left->str() + " < " + "(" + right->str() + ")";
        }
        return left->str() + " < " + right->str();
    }
    int getWeight() const { return weight; }
};

class EquivalenceNode : public BinNode {
private:
    const int weight = 1;
public:
    EquivalenceNode(BoolNode *L, BoolNode *R) : BinNode(L, R) {}
    bool calc() const { return left->calc() == right->calc(); }
    std::string str() const {
        if (left->getWeight() < weight && right->getWeight() < weight) {
            return "(" + left->str() + ")" + " = " + "(" + right->str() + ")";
        }
        if (left->getWeight() < weight && right->getWeight() >= weight) {
            return "(" + left->str() + ")" + " = " + right->str();
        }
        if (left->getWeight() >= weight && right->getWeight() < weight) {
            return left->str() + " = " + "(" + right->str() + ")";
        }
        return left->str() + " = " + right->str();
    }
    int getWeight() const { return weight; }
};

class ShefferNode : public BinNode {
private:
    const int weight = 4;
public:
    ShefferNode(BoolNode *L, BoolNode *R) : BinNode(L, R) {}
    bool calc() const { return !(left->calc() && right->calc()); }
    std::string str() const {
        if (left->getWeight() < weight && right->getWeight() < weight) {
            return "(" + left->str() + ")" + " | " + "(" + right->str() + ")";
        }
        if (left->getWeight() < weight && right->getWeight() >= weight) {
            return "(" + left->str() + ")" + " | " + right->str();
        }
        if (left->getWeight() >= weight && right->getWeight() < weight) {
            return left->str() + " | " + "(" + right->str() + ")";
        }
        return left->str() + " | " + right->str();
    }
    int getWeight() const { return weight; }
};

class PierceNode : public BinNode {
private:
    const int weight = 3;
public:
    PierceNode(BoolNode *L, BoolNode *R) : BinNode(L, R) {}
    bool calc() const { return !(left->calc() || right->calc()); }
    std::string str() const {
        if (left->getWeight() < weight && right->getWeight() < weight) {
            return "(" + left->str() + ")" + " ^ " + "(" + right->str() + ")";
        }
        if (left->getWeight() < weight && right->getWeight() >= weight) {
            return "(" + left->str() + ")" + " ^ " + right->str();
        }
        if (left->getWeight() >= weight && right->getWeight() < weight) {
            return left->str() + " ^ " + "(" + right->str() + ")";
        }
        return left->str() + " ^ " + right->str();
    }
    int getWeight() const { return weight; }
};

// ================================================================================

// ==================================Unary Node===================================

class NegationNode : public BoolNode {
private:
    BoolNode *next;
    const int weight = 8;
public:
    NegationNode(BoolNode *node) : next(node) {}

    bool calc() const { return !(next->calc()); }
    std::string str() const {
        if (next->getWeight() < weight) {
            return "(~" + next->str() + ")";
        }
        return "~" + next->str();
    }
    int getWeight() const { return weight; }
};


#endif //TASK5_BOOLCALC_DEYMOS01_BOOLNODE_H