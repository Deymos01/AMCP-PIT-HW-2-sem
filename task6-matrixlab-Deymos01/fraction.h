#ifndef TASK6_MATRIXLAB_DEYMOS01_FRACTION_H
#define TASK6_MATRIXLAB_DEYMOS01_FRACTION_H

#include <string>
#include <iostream>
#include "biginteger.h"

class Fraction {
private:
    BigInteger numerator;
    BigInteger denominator;

    void swap(Fraction &other) {
        std::swap(denominator, other.denominator);
        std::swap(numerator, other.numerator);
    }

    void simplify();
public:
    // __________Constructors__________
    Fraction() : numerator(0), denominator(1) {}
    Fraction(BigInteger num, BigInteger den) : numerator(std::move(num)), denominator(std::move(den)) { simplify(); }
    Fraction(BigInteger num) : numerator(std::move(num)), denominator(1) {}
    Fraction(int num) : Fraction(BigInteger(num)) {}
    Fraction(int num, int den) : Fraction(BigInteger(num), BigInteger(den)) {}
    Fraction(const std::string &frac);

    // Copy constructor
    Fraction(const Fraction &other) : numerator(other.numerator), denominator(other.denominator) {}
    // Move constructor
    Fraction(Fraction &&other) noexcept {
        swap(other);
    }
    // ----------------------------------

    // __________Operators__________
    Fraction &operator=(const Fraction &other);
    Fraction &operator=(Fraction &&other) noexcept;
    Fraction operator-() const;
    Fraction operator+() const;
    Fraction &operator++();
    Fraction operator++(int);
    Fraction &operator--();
    Fraction operator--(int);
    Fraction &operator+=(const Fraction &);
    Fraction &operator-=(const Fraction &);
    Fraction &operator*=(const Fraction &);
    Fraction &operator/=(const Fraction &);
    Fraction &operator%=(const Fraction &);
    // -------------------------------

    // ________COMPARISON_OPERATORS____________
    bool operator==(const Fraction &) const;
    bool operator!=(const Fraction &) const;
    bool operator<(const Fraction &) const;
    bool operator<=(const Fraction &) const;
    bool operator>(const Fraction &) const;
    bool operator>=(const Fraction &) const;
    // ________________________________________

    // ________________Output________________
    const BigInteger &getNumerator() const { return numerator; }
    friend std::string to_string(const Fraction &data);
    friend std::ostream &operator<<(std::ostream &out, const Fraction &data);
    // ______________________________________
};

// ________ARITHMETICS_OPERATORS________
Fraction operator+(const Fraction &left, const Fraction &right);
Fraction operator-(const Fraction &left, const Fraction &right);
Fraction operator*(const Fraction &left, const Fraction &right);
Fraction operator/(const Fraction &left, const Fraction &right);
Fraction operator%(const Fraction &left, const Fraction &right);
// -------------------------------------
#endif //TASK6_MATRIXLAB_DEYMOS01_FRACTION_H
