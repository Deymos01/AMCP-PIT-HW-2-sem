#ifndef TASK6_MATRIXLAB_DEYMOS01_MATRIX_H
#define TASK6_MATRIXLAB_DEYMOS01_MATRIX_H

#include <vector>
#include <iostream>

#include "fraction.h"
#include "biginteger.h"

class Matrix {
private:
    std::vector<std::vector<Fraction>> array_;

    void swap(Matrix &other) {
        std::swap(array_, other.array_);
    }
public:
    // ________CONSTRUCTORS________
    Matrix() {}
    Matrix(const Fraction &number, const size_t &rows, const size_t &cols);
    Matrix(const std::vector<std::vector<Fraction>> &inputData);
    // Copy constructor
    Matrix(const Matrix &other) : array_(other.array_) {}
    // Move constructor
    Matrix(Matrix &&other) noexcept { swap(other); }
    // ____________________________

    Matrix &operator=(const Matrix &other);

    // ________ARITHMETIC_OPERATORS__________
    friend Matrix operator+(const Matrix &left, const Matrix &right);
    friend Matrix operator-(const Matrix &left, const Matrix &right);
    friend Matrix mul(const Matrix &left, const Matrix &right);
    friend Matrix div(const Matrix &left, const Matrix &right);
    friend Matrix operator*(const Matrix &left, const Matrix &right);
    friend Matrix operator*(const Matrix &matrix, const Fraction &frac);
    friend Matrix operator*(const Fraction &frac, const Matrix &matrix);
    friend Matrix operator/(const Matrix &left, const Matrix &right);
    friend Matrix operator-(const Matrix &matrix);
    // _____________________________________

    // _____________MATRIX_ACTIONS______________
    friend Matrix inv(const Matrix &matrix);
    friend Fraction det(const Matrix &matrix);
    friend Fraction sum(const Matrix &matrix);
    friend Fraction prod(const Matrix &matrix);
    friend Matrix min(const Matrix &left, const Matrix &right);
    friend Matrix max(const Matrix &left, const Matrix &right);
    friend Matrix transpose(const Matrix &matrix);
    friend Matrix makeRowEchelonForm(const Matrix &matrix);
    friend Fraction minor(Matrix &matrix, const Fraction& row, const Fraction& col);
    // _________________________________________

    // ________MATRIX_CONCATENATION________
    friend Matrix vertcat(const std::vector<Matrix> &matrices);
    friend Matrix horzcat(const std::vector<Matrix> &matrices);
    // ____________________________________

    // ________OUTPUT________
    std::vector<Fraction> &operator[](const size_t &index);
    friend std::ostream &operator<<(std::ostream &out, const Matrix &data);
    // ______________________

    // _________UTILS_________
    size_t getNumberRows() const;
    size_t getNumberCols() const;
    size_t getNumberElems() const;
    void shiftRows(const size_t &col);
    // _______________________
};

#endif //TASK6_MATRIXLAB_DEYMOS01_MATRIX_H
