#ifndef TASK5_BOOLCALC_DEYMOS01_ERRORS_H
#define TASK5_BOOLCALC_DEYMOS01_ERRORS_H

#include <string>

class Error {
public:
    virtual std::string what() const {
        return std::string("Unknown error");
    }
};

class ErrorSymbol : public Error {
private:
    std::string str_;
public:
    ErrorSymbol(const char *s) : str_(s) {}
    std::string what() const {
        return "Unexpected symbol: " + str_;
    }
};

class ErrorParameter : public Error {
private:
    std::string str_;
public:
    ErrorParameter(const char *s) : str_(s) {}
    std::string what() const {
        return "Unexpected parameter name: '" + str_ + "'";
    }
};

class ErrorBracketsClose : public Error {
private:
    std::string str_;
public:
    ErrorBracketsClose(const char *s) : str_(s) {}

    std::string what() const {
        return std::string("Error with brackets. There are no ')' in '") + str_ + "'";
    }
};

class ErrorBracketsOpen : public Error {
private:
    std::string str_;
public:
    ErrorBracketsOpen(const char *s) : str_(s) {}

    std::string what() const {
        return std::string("Error with brackets. There are no '(' in '") + str_ + "'";
    }
};

class ErrorNoOperand : public Error {
public:
    std::string what() const {
        return std::string("There is no operand between parameters");
    }
};

class ErrorDNF : public Error {};

class ErrorCNF : public Error {};

#endif //TASK5_BOOLCALC_DEYMOS01_ERRORS_H
