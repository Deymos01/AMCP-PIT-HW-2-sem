#ifndef TASK6_MATRIXLAB_DEYMOS01_ERRORS_H
#define TASK6_MATRIXLAB_DEYMOS01_ERRORS_H

#include <string>

class Error {
public:
    virtual std::string what() const {
        return std::string("Unknown error");
    }
};

class WrongArgument : public Error {
private:
    size_t number;
public:
    WrongArgument(size_t numberOfArgument) : number(numberOfArgument) {}

    std::string what() const {
        return "Invalid argument: arg #" + std::to_string(number);
    }
};

class OutOfRange : public Error {
    std::string what() const {
        return "Out of range";
    }
};

class ClosedBracketError : public Error {
    std::string what() const {
        return "Missed ')' in input line";
    }
};

class OpenedBracketError : public Error {
    std::string what() const {
        return "Missed '(' in input line";
    }
};

class SumMatrixError : public Error {
    std::string what() const {
        return "Can't add these matrices";
    }
};

class MinusMatrixError : public Error {
    std::string what() const {
        return "Can't subtract these matrices";
    }
};

class MulMatrixError : public Error {
    std::string what() const {
        return "Can't multiply these matrices";
    }
};

class DivMatrixError : public Error {
    std::string what() const {
        return "Can't divide these matrices";
    }
};

class ConcatenationError : public Error {
    std::string what() const {
        return "Can't concatenate these matrices";
    }
};

class MinimumMatrixError : public Error {
public:
    std::string what() const {
        return "Can't calculate minimum matrix for these matrices";
    }
};

class MaximumMatrixError : public Error {
public:
    std::string what() const {
        return "Can't calculate maximum matrix for these matrices";
    }
};

class CreateMatrixError : public Error {
    std::string what() const {
        return "Can't create a matrix by the input data";
    }
};

class DeterminantError : public Error {
    std::string what() const {
        return "This matrix is not square";
    }
};

class SingularMatrix : public Error {
    std::string what() const {
        return "Can't invert this matrix, because it's singular";
    }
};

// ====================================================

class CalcNodeError : public Error {
    virtual std::string what() const {
        return "Can't do the action";
    }
};

class ParameterError : public CalcNodeError {
    std::string what() const {
        return "Expected name of parameter";
    }
};

class AssignmentValueError : public CalcNodeError {
    std::string what() const {
        return "Expected value";
    }
};

class UndefinedParameter : public ParameterError {
    std::string what() const {
        return "Undefined parameter";
    }
};

#endif //TASK6_MATRIXLAB_DEYMOS01_ERRORS_H
