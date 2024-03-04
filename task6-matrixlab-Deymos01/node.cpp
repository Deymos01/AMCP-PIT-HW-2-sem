#include <map>
#include "node.h"

namespace global {
    std::map<std::string, Matrix> Workspace;
}

void getArgs(std::stack<Node *> &stack, Data &params, size_t quantity) {
    params.parameter1 = dynamic_cast<DataNode *>(stack.top());
    if (params.parameter1 == nullptr) {
        throw WrongArgument(1);
    }
    stack.pop();

    if (quantity > 1) {
        params.parameter2 = dynamic_cast<DataNode *>(stack.top());
        if (params.parameter2 == nullptr) {
            throw WrongArgument(2);
        }
        stack.pop();

        if (quantity > 2) {
            params.parameter3 = dynamic_cast<DataNode *>(stack.top());
            if (params.parameter3 == nullptr) {
                throw WrongArgument(3);
            }
            stack.pop();
        }
    }
}

//=======================================================

Node *AssignmentNode::calc(std::stack<Node *> &stack) {
    Data data;
    data.parameter1 = dynamic_cast<ParamNode *>(dynamic_cast<DataNode *>(stack.top()));
    if (data.parameter1 == nullptr) {
        throw ParameterError();
    }
    stack.pop();

    data.parameter2 = dynamic_cast<DataNode *>(stack.top());
    if (data.parameter2 == nullptr) {
        throw AssignmentValueError();
    }
    stack.pop();

    global::Workspace[dynamic_cast<ParamNode *>(data.parameter1)->getName()] = data.parameter2->getValue();

    return new Node();
}

Node *PlusNode::calc(std::stack<Node *> &stack) {
    Data data;
    try {
        getArgs(stack, data, 2);
    } catch (...) {
        throw;
    }

    Matrix A;
    Matrix B;

    try {
        A = data.parameter1->getValue();
        B = data.parameter2->getValue();
    } catch (...) {
        throw UndefinedParameter();
    }

    try {
        return new ValueNode(A + B);
    } catch (...) {
        throw;
    }
}

Node *MinusNode::calc(std::stack<Node *> &stack) {
    Data data;
    data.parameter1 = dynamic_cast<DataNode *>(stack.top());
    if (data.parameter1 == nullptr) {
        throw WrongArgument(1);
    }
    stack.pop();
    Matrix A;
    try {
        A = data.parameter1->getValue();
    } catch (...) {
        throw WrongArgument(1);
    }

    if (dynamic_cast<DataNode *>(stack.top()) == nullptr) {
        // Unar minus
        try {
            return new ValueNode(-A);
        } catch (...) {
            throw CalcNodeError();
        }
    } else {
        // Binary minus
        data.parameter2 = dynamic_cast<DataNode *>(stack.top());
        stack.pop();
        Matrix B;

        try {
            B = data.parameter2->getValue();
        } catch (...) {
            throw WrongArgument(2);
        }

        try {
            return new ValueNode(A - B);
        } catch (...) {
            throw;
        }
    }
}

Node *MultiplicationNode::calc(std::stack<Node *> &stack) {
    Data data;

    try {
        getArgs(stack, data, 2);
    } catch (...) {
        throw;
    }

    Matrix A;
    Matrix B;

    try {
        A = data.parameter1->getValue();
        B = data.parameter2->getValue();
    } catch (...) {
        throw UndefinedParameter();
    }

    if (A.getNumberElems() == 1) {
        Fraction number = A[0][0];
        try {
            return new ValueNode(number * B);
        } catch (...) {
            throw CalcNodeError();
        }
    } else if (B.getNumberElems() == 1) {
        Fraction number = B[0][0];
        try {
            return new ValueNode(A * number);
        } catch (...) {
            throw CalcNodeError();
        }
    }

    try {
        return new ValueNode(A * B);
    } catch (...) {
        throw;
    }
}

Node *DivisionNode::calc(std::stack<Node *> &stack) {
    Data data;

    try {
        getArgs(stack, data, 2);
    } catch (...) {
        throw;
    }

    Matrix A;
    Matrix B;

    try {
        A = data.parameter1->getValue();
        B = data.parameter2->getValue();
    } catch (...) {
        throw UndefinedParameter();
    }

    try {
        return new ValueNode(A / B);
    } catch (...) {
        throw DivMatrixError();
    }
}

Node *MultElemNode::calc(std::stack<Node *> &stack) {
    Data data;

    try {
        getArgs(stack, data, 2);
    } catch (...) {
        throw;
    }

    Matrix A;
    Matrix B;

    try {
        A = data.parameter1->getValue();
        B = data.parameter2->getValue();
    } catch (...) {
        throw UndefinedParameter();
    }

    try {
        return new ValueNode(mul(A, B));
    } catch (...) {
        throw;
    }
}

Node *DivElemNode::calc(std::stack<Node *> &stack) {
    Data data;

    try {
        getArgs(stack, data, 2);
    } catch (...) {
        throw;
    }

    Matrix A;
    Matrix B;

    try {
        A = data.parameter1->getValue();
        B = data.parameter2->getValue();
    } catch (...) {
        throw UndefinedParameter();
    }

    try {
        return new ValueNode(div(A, B));
    } catch (...) {
        throw;
    }
}

//=======================================================

Node *ZerosNode::calc(std::stack<Node *> &stack) {
    Data data;

    try {
        getArgs(stack, data, 2);
    } catch (...) {
        throw;
    }

    if (data.parameter1 == nullptr || data.parameter2 == nullptr ||
        data.parameter1->getValue().getNumberElems() != 1 ||
        data.parameter2->getValue().getNumberElems() != 1) {
        throw CreateMatrixError();
    }

    size_t row = data.parameter1->getValue()[0][0].getNumerator();
    size_t col = data.parameter2->getValue()[0][0].getNumerator();

    return new ValueNode(Matrix(0, row, col));
}

Node *OnesNode::calc(std::stack<Node *> &stack) {
    Data data;

    try {
        getArgs(stack, data, 2);
    } catch (...) {
        throw;
    }

    if (data.parameter1 == nullptr || data.parameter2 == nullptr ||
        data.parameter1->getValue().getNumberElems() != 1 ||
        data.parameter2->getValue().getNumberElems() != 1) {
        throw CreateMatrixError();
    }

    size_t row = data.parameter1->getValue()[0][0].getNumerator();
    size_t col = data.parameter2->getValue()[0][0].getNumerator();

    return new ValueNode(Matrix(1, row, col));
}

Node *EyeNode::calc(std::stack<Node *> &stack) {
    Data data;
    data.parameter1 = dynamic_cast<DataNode *>(stack.top());
    if (data.parameter1 == nullptr || data.parameter1->getValue().getNumberElems() != 1) {
        throw CreateMatrixError();
    }

    stack.pop();

    size_t sz = data.parameter1->getValue()[0][0].getNumerator();

    Matrix res(0, sz, sz);
    for (size_t i = 0; i < sz; ++i) {
        res[i][i] = 1;
    }
    return new ValueNode(res);
}

Node *LinspaceNode::calc(std::stack<Node *> &stack) {
    Data data;

    try {
        getArgs(stack, data, 3);
    } catch (...) {
        throw;
    }

    Fraction begin = data.parameter1->getValue()[0][0];
    Fraction end = data.parameter2->getValue()[0][0];
    Fraction quantity = data.parameter3->getValue()[0][0];

    Fraction step = (end - begin + 1) / quantity;

    std::vector<Fraction> result(quantity.getNumerator(), begin);
    for (int i = 1; i < result.size(); ++i) {
        result[i] += i * step;
    }
    return new ValueNode(Matrix(std::vector<std::vector<Fraction>>(1, result)));
}

Node *VertcatNode::calc(std::stack<Node *> &stack) {
    std::vector<Matrix> matrices;
    size_t counter = 0;
    while (dynamic_cast<DataNode *>(stack.top()) != nullptr) {
        matrices.emplace_back(dynamic_cast<DataNode *>(stack.top())->getValue());
        delete stack.top();
        stack.pop();
        ++counter;
    }
    if (counter == 0) {
        throw ConcatenationError();
    }
    return new ValueNode(vertcat(matrices));
}

Node *HorzcatNode::calc(std::stack<Node *> &stack) {
    std::vector<Matrix> matrices;
    size_t counter = 0;
    while (dynamic_cast<DataNode *>(stack.top()) != nullptr) {
        matrices.emplace_back(dynamic_cast<DataNode *>(stack.top())->getValue());
        delete stack.top();
        stack.pop();
        ++counter;
    }
    if (counter == 0) {
        throw ConcatenationError();
    }
    return new ValueNode(horzcat(matrices));
}

Node *TransposeNode::calc(std::stack<Node *> &stack) {
    Data data;
    data.parameter1 = dynamic_cast<DataNode *>(stack.top());
    if (data.parameter1 == nullptr) {
        throw WrongArgument(1);
    }
    stack.pop();

    return new ValueNode(transpose(data.parameter1->getValue()));
}

Node *DeterminantNode::calc(std::stack<Node *> &stack) {
    Data data;
    data.parameter1 = dynamic_cast<DataNode *>(stack.top());
    if (data.parameter1 == nullptr) {
        throw WrongArgument(1);
    }
    stack.pop();

    return new ValueNode(Matrix(det(data.parameter1->getValue()), 1, 1));
}

Node *InverseNode::calc(std::stack<Node *> &stack) {
    Data data;
    data.parameter1 = dynamic_cast<DataNode *>(stack.top());
    if (data.parameter1 == nullptr) {
        throw WrongArgument(1);
    }
    stack.pop();

    return new ValueNode(inv(data.parameter1->getValue()));
}

Node *SumNode::calc(std::stack<Node *> &stack) {
    Data data;
    data.parameter1 = dynamic_cast<DataNode *>(stack.top());
    if (data.parameter1 == nullptr) {
        throw WrongArgument(1);
    }
    stack.pop();

    return new ValueNode(Matrix(sum(data.parameter1->getValue()), 1, 1));
}

Node *ProductNode::calc(std::stack<Node *> &stack) {
    Data data;
    data.parameter1 = dynamic_cast<DataNode *>(stack.top());
    if (data.parameter1 == nullptr) {
        throw WrongArgument(1);
    }
    stack.pop();

    return new ValueNode(Matrix(prod(data.parameter1->getValue()), 1, 1));
}

Node *MinimumNode::calc(std::stack<Node *> &stack) {
    Data data;

    try {
        getArgs(stack, data, 2);
    } catch (...) {
        throw;
    }

    Matrix A;
    Matrix B;

    try {
        A = data.parameter1->getValue();
        B = data.parameter2->getValue();
    } catch (...) {
        throw;
    }

    try {
        return new ValueNode(min(A, B));
    } catch (...) {
        throw;
    }
}

Node *MaximumNode::calc(std::stack<Node *> &stack) {
    Data data;

    try {
        getArgs(stack, data, 2);
    } catch (...) {
        throw;
    }

    Matrix A;
    Matrix B;

    try {
        A = data.parameter1->getValue();
        B = data.parameter2->getValue();
    } catch (...) {
        throw;
    }

    try {
        return new ValueNode(max(A, B));
    } catch (...) {
        throw;
    }
}

Node *RowEchelonFormNode::calc(std::stack<Node *> &stack) {
    Data data;
    data.parameter1 = dynamic_cast<DataNode *>(stack.top());
    if (data.parameter1 == nullptr) {
        throw WrongArgument(1);
    }
    stack.pop();

    return new ValueNode(makeRowEchelonForm(data.parameter1->getValue()));
}

Node *RankNode::calc(std::stack<Node *> &stack) {
    Data data;
    data.parameter1 = dynamic_cast<DataNode *>(stack.top());
    if (data.parameter1 == nullptr) {
        throw WrongArgument(1);
    }
    stack.pop();

    Matrix curr(makeRowEchelonForm(data.parameter1->getValue()));
    size_t countZeroRows = (curr.getNumberRows() > curr.getNumberCols()) ? curr.getNumberRows() - curr.getNumberCols() : 0;
    size_t index = std::min(curr.getNumberCols(), curr.getNumberCols()) - 1;
    int differ = (curr.getNumberCols() > curr.getNumberRows()) ? (int) (curr.getNumberCols() - curr.getNumberRows()) : 0;
    for (; index > 0; --index, ++countZeroRows) {
        if (curr[index - differ][index] != BigInteger(0)) {
            break;
        }
    }
    return new ValueNode(Matrix(static_cast<int>(curr.getNumberRows() - countZeroRows), 1, 1));
}

Node *MinorNode::calc(std::stack<Node *> &stack) {
    // arg1 - matrix
    // arg2, arg3 - coords (row and col) of elem
    // *coords start with 1

    Data data;

    try {
        getArgs(stack, data, 3);
    } catch (...) {
        throw;
    }

    Matrix A;
    Fraction row;
    Fraction col;

    try {
        A = data.parameter1->getValue();
        row = data.parameter2->getValue()[0][0] - 1;
        col = data.parameter3->getValue()[0][0] - 1;
    } catch (...) {
        throw UndefinedParameter();
    }
    if (row < 0 || row >= static_cast<int>(A.getNumberRows()) ||
        col < 0 || col >= static_cast<int>(A.getNumberCols())) {
        throw CalcNodeError();
    }

    return new ValueNode(Matrix(minor(A, row, col), 1, 1));
}

Node *CofactorNode::calc(std::stack<Node *> &stack) {
    // arg1 - matrix
    // arg2, arg3 - coords (row and col) of elem
    // *coords start with 1

    Data data;

    try {
        getArgs(stack, data, 3);
    } catch (...) {
        throw;
    }

    Matrix A;
    Fraction row;
    Fraction col;

    try {
        A = data.parameter1->getValue();
        row = data.parameter2->getValue()[0][0] - 1;
        col = data.parameter3->getValue()[0][0] - 1;
    } catch (...) {
        throw UndefinedParameter();
    }
    if (row < 0 || row >= static_cast<int>(A.getNumberRows()) ||
        col < 0 || col >= static_cast<int>(A.getNumberCols())) {
        throw CalcNodeError();
    }

    Fraction mul(1);

    if ((row + col) % 2 != 0) {
        mul = -1;
    }

    return new ValueNode(Matrix(mul * minor(A, row, col), 1, 1));
}

//=======================================================

Node *DisplayNode::calc(std::stack<Node *> &stack) {
    std::string resultName = "ans";
    Data data;
    data.parameter1 = dynamic_cast<DataNode *>(stack.top());
    stack.pop();
    if (dynamic_cast<ParamNode *>(data.parameter1) != nullptr) {
        resultName = dynamic_cast<ParamNode *>(data.parameter1)->getName();
        if (global::Workspace.find(resultName) == global::Workspace.end()) {
            throw UndefinedParameter();
        }
        *out_ << resultName << " = ";
        Matrix res = global::Workspace[resultName];
        if (res.getNumberRows() == 1 && res.getNumberCols() == 1) {
            *out_ << res << std::endl;
        } else {
            *out_ << "[" << std::endl << res << std::endl << "]" << std::endl;
        }
        return new Node();
    }
    if (dynamic_cast<ValueNode *>(data.parameter1) == nullptr) {
        throw CalcNodeError();
    }
    *out_ << resultName << " = ";
    Matrix res = dynamic_cast<ValueNode *>(data.parameter1)->getValue();
    if (res.getNumberRows() == 1 && res.getNumberCols() == 1) {
        *out_ << res << std::endl;
    } else {
        *out_ << "[" << std::endl << res << std::endl << "]" << std::endl;
    }
    return new Node();
}