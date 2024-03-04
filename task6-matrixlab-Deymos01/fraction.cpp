#include "fraction.h"

// string constructor
Fraction::Fraction(const std::string &frac) {
    numerator = BigInteger(frac.substr(0, frac.find('/')));
    if (frac.find('/') == std::string::npos) {
        denominator = BigInteger(1);
    } else {
        denominator = BigInteger(frac.substr(frac.find('/') + 1));
        simplify();
    }
}
// ------------------

void Fraction::simplify() {
    BigInteger gcd = getGCD(numerator, denominator);
    numerator /= gcd;
    denominator /= gcd;

    if (!numerator.getSign() && !denominator.getSign()) {
        numerator.setSign(true);
        denominator.setSign(true);
    } else if (!denominator.getSign()) {
        numerator.setSign(false);
        denominator.setSign(true);
    }
}

// ________COMPARISON_OPERATORS____________
bool Fraction::operator==(const Fraction &other) const {
    return numerator == other.numerator && denominator == other.denominator;
}
bool Fraction::operator!=(const Fraction &other) const {
    return !(*this == other);
}
bool Fraction::operator<(const Fraction &other) const {
    return numerator * other.denominator < other.numerator * denominator;
}
bool Fraction::operator<=(const Fraction &other) const {
    return (*this == other) || (*this < other);
}
bool Fraction::operator>(const Fraction &other) const {
    return other < (*this);
}
bool Fraction::operator>=(const Fraction &other) const {
    return other <= (*this);
}
// ________________________________________

Fraction &Fraction::operator=(const Fraction &other) {
    if (this != &other) {
        Fraction tmp(other);
        swap(tmp);
    }
    return *this;
}
Fraction &Fraction::operator=(Fraction &&other) noexcept {
    if (this != &other) {
        swap(other);
    }
    return *this;
}
// ________ARITHMETICS_OPERATORS________
Fraction Fraction::operator-() const {
    return Fraction(-numerator, denominator);
}
Fraction Fraction::operator+() const {
    return *this;
}
Fraction &Fraction::operator++() { //prefix
    ++numerator;
    return *this;
}
Fraction Fraction::operator++(int) { //postfix
    Fraction tmp(*this);
    ++(*this);
    return tmp;
}
Fraction &Fraction::operator--() { // prefix
    --numerator;
    return *this;
}
Fraction Fraction::operator--(int) { // postfix
    Fraction tmp(*this);
    --(*this);
    return tmp;
}
Fraction &Fraction::operator+=(const Fraction &other) {
    numerator *= other.denominator;
    numerator += denominator * other.numerator;
    denominator *= other.denominator;
    simplify();
    return *this;
}
Fraction &Fraction::operator-=(const Fraction &other) {
    numerator *= other.denominator;
    numerator -= denominator * other.numerator;
    denominator *= other.denominator;
    simplify();
    return *this;
}
Fraction &Fraction::operator*=(const Fraction &other) {
    numerator *= other.numerator;
    denominator *= other.denominator;
    simplify();
    return *this;
}
Fraction &Fraction::operator/=(const Fraction &other) {
    numerator *= other.denominator;
    denominator *= other.numerator;
    simplify();
    return *this;
}

Fraction &Fraction::operator%=(const Fraction &other) {
    if (denominator != BigInteger(1) || other.denominator != BigInteger(1)) {
        throw;
    }
    numerator %= other.numerator;
    return *this;
}

Fraction operator+(const Fraction &left, const Fraction &right) {
    Fraction tmp(left);
    tmp += right;
    return tmp;
}
Fraction operator-(const Fraction &left, const Fraction &right) {
    Fraction tmp(left);
    tmp -= right;
    return tmp;
}
Fraction operator*(const Fraction &left, const Fraction &right) {
    Fraction tmp(left);
    tmp *= right;
    return tmp;
}
Fraction operator/(const Fraction &left, const Fraction &right) {
    Fraction tmp(left);
    tmp /= right;
    return tmp;
}
Fraction operator%(const Fraction &left, const Fraction &right) {
    Fraction tmp(left);
    tmp %= right;
    return tmp;
}
// -------------------------------------

// ________________Output________________
std::ostream &operator<<(std::ostream &out, const Fraction &data) {
    out << to_string(data);
    return out;
}
std::string to_string(const Fraction &data) {
    return to_string(data.numerator) + ((data.denominator == BigInteger(1)) ? "" : '/' + to_string(data.denominator));
}
// ______________________________________