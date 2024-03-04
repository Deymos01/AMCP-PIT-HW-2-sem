#include <iostream>
#include <iomanip>
#include "matrix.h"
#include "fraction.h"
#include "errors.h"

// __________CONSTRUCTORS__________
Matrix::Matrix(const Fraction &number, const size_t &rows, const size_t &cols) {
    array_ = std::vector<std::vector<Fraction>>(rows, std::vector<Fraction>(cols, number));
}
Matrix::Matrix(const std::vector<std::vector<Fraction>> &inputData) {
    for (const auto &elem: inputData) {
        if (elem.size() != inputData[0].size()) {
            throw CreateMatrixError();
        }
    }
    array_ = inputData;
}
// ________________________________

Matrix &Matrix::operator=(const Matrix &other) {
    if (this != &other) {
        Matrix tmp(other);
        swap(tmp);
    }
    return *this;
}

// ________ARITHMETIC_OPERATORS__________
Matrix operator+(const Matrix &left, const Matrix &right) {
    if (left.getNumberRows() != right.getNumberRows() || left.getNumberCols() != right.getNumberCols()) {
        throw SumMatrixError();
    }
    Matrix result(left);
    for (int i = 0; i < left.getNumberRows(); ++i) {
        for (int j = 0; j < right.getNumberCols(); ++j) {
            result.array_[i][j] += right.array_[i][j];
        }
    }
    return result;
}
Matrix operator-(const Matrix &left, const Matrix &right) {
    if (left.getNumberRows() != right.getNumberRows() || left.getNumberCols() != right.getNumberCols()) {
        throw MinusMatrixError();
    }
    Matrix result(left);
    for (int i = 0; i < left.getNumberRows(); ++i) {
        for (int j = 0; j < right.getNumberCols(); ++j) {
            result.array_[i][j] -= right.array_[i][j];
        }
    }
    return result;
}
Matrix mul(const Matrix &left, const Matrix &right) {
    if (left.getNumberRows() != right.getNumberRows() || left.getNumberCols() != right.getNumberCols()) {
        throw MulMatrixError();
    }
    Matrix result(left);
    for (int i = 0; i < left.getNumberRows(); ++i) {
        for (int j = 0; j < right.getNumberCols(); ++j) {
            result.array_[i][j] *= right.array_[i][j];
        }
    }
    return result;
}
Matrix div(const Matrix &left, const Matrix &right) {
    if (left.getNumberRows() != right.getNumberRows() || left.getNumberCols() != right.getNumberCols()) {
        throw DivMatrixError();
    }
    Matrix result(left);
    for (int i = 0; i < left.getNumberRows(); ++i) {
        for (int j = 0; j < right.getNumberCols(); ++j) {
            result.array_[i][j] /= right.array_[i][j];
        }
    }
    return result;
}
Matrix operator*(const Matrix &left, const Matrix &right) {
    if (left.getNumberCols() != right.getNumberRows()) {
        throw MulMatrixError();
    }

    Matrix result(Fraction("0"), left.array_.size(), right.array_[0].size());
    for (size_t i = 0; i < result.getNumberRows(); ++i) {
        for (size_t j = 0; j < result.getNumberCols(); ++j) {
            for (size_t k = 0; k < left.getNumberCols(); ++k) {
                result.array_[i][j] += left.array_[i][k] * right.array_[k][j];
            }
        }
    }

    return result;
}
Matrix operator*(const Matrix &matrix, const Fraction &frac) {
    Matrix result(matrix);
    for (size_t i = 0; i < matrix.getNumberRows(); ++i) {
        for (size_t j = 0; j < matrix.getNumberCols(); ++j) {
            result.array_[i][j] *= frac;
        }
    }
    return result;
}
Matrix operator*(const Fraction &frac, const Matrix &matrix) {
    return (matrix * frac);
}
Matrix operator/(const Matrix &left, const Matrix &right) {
    return left * inv(right);
}
Matrix operator-(const Matrix &matrix) {
    Matrix result(matrix);
    for (size_t i = 0; i < matrix.getNumberRows(); ++i) {
        for (size_t j = 0; j < matrix.getNumberCols(); ++j) {
            result.array_[i][j] = -result.array_[i][j];
        }
    }
    return result;
}
// _____________________________________

// _____________MATRIX_ACTIONS______________
Matrix inv(const Matrix &matrix) {
    if (det(matrix) == 0) {
        throw SingularMatrix();
    }

    Matrix expandedMatrix;
    expandedMatrix.array_ = std::vector<std::vector<Fraction>>(matrix.getNumberRows(),
                                                               std::vector<Fraction>(2 * matrix.getNumberCols(), 0));

    for (size_t i = 0; i < matrix.getNumberRows(); ++i) {
        for (size_t j = 0; j < expandedMatrix.getNumberCols(); ++j) {
            if (j < matrix.getNumberCols()) {
                expandedMatrix.array_[i][j] = matrix.array_[i][j];
            } else if (i == j - matrix.getNumberCols()) {
                expandedMatrix.array_[i][j] = 1;
            } else {
                expandedMatrix.array_[i][j] = 0;
            }
        }
    }
    expandedMatrix = makeRowEchelonForm(expandedMatrix);
    for (int col = (int) (matrix.getNumberRows()) - 1; col > 0; --col) {
        for (int row = col - 1; row >= 0; --row) {
            if (expandedMatrix.array_[row][col] == 0) {
                continue;
            }
            Fraction lineCoef = expandedMatrix.array_[row][col] / expandedMatrix.array_[col][col];
            for (size_t elem = col; elem < expandedMatrix.getNumberCols(); ++elem) {
                expandedMatrix.array_[row][elem] -= expandedMatrix.array_[col][elem] * lineCoef;
            }
        }
    }

    Matrix result(matrix);
    for (size_t i = 0; i < matrix.getNumberRows(); ++i) {
        for (size_t j = 0; j < matrix.getNumberCols(); ++j) {
            Fraction resFrac = expandedMatrix.array_[i][expandedMatrix.getNumberCols() -
                                                        expandedMatrix.getNumberRows() + j];
            if (expandedMatrix.array_[i][i] != 1) {
                resFrac /= expandedMatrix.array_[i][i];
            }
            result.array_[i][j] = resFrac;
        }
    }
    return result;
}
Fraction det(const Matrix &matrix) {
    if (matrix.getNumberCols() != matrix.getNumberRows()) {
        throw DeterminantError();
    }

    Matrix triang = makeRowEchelonForm(matrix);
    Fraction result(1);
    for (size_t row = 0; row < matrix.getNumberRows(); ++row) {
        result *= triang.array_[row][row];
    }
    return result;
}
Fraction sum(const Matrix &matrix) {
    Fraction result;
    for (size_t i = 0; i < matrix.getNumberRows(); ++i) {
        for (size_t j = 0; j < matrix.getNumberCols(); ++j) {
            result += matrix.array_[i][j];
        }
    }
    return result;
}
Fraction prod(const Matrix &matrix) {
    Fraction result(1);
    for (size_t i = 0; i < matrix.getNumberRows(); ++i) {
        for (size_t j = 0; j < matrix.getNumberCols(); ++j) {
            result *= matrix.array_[i][j];
        }
    }
    return result;
}
Matrix min(const Matrix &left, const Matrix &right) {
    if (left.getNumberRows() != right.getNumberRows() || left.getNumberCols() != right.getNumberCols()) {
        throw MinimumMatrixError();
    }
    Matrix result(left);

    for (size_t i = 0; i < left.getNumberRows(); ++i) {
        for (size_t j = 0; j < left.getNumberCols(); ++j) {
            result.array_[i][j] = std::min(result.array_[i][j], right.array_[i][j]);
        }
    }

    return result;
}
Matrix max(const Matrix &left, const Matrix &right) {
    if (left.getNumberRows() != right.getNumberRows() || left.getNumberCols() != right.getNumberCols()) {
        throw MaximumMatrixError();
    }
    Matrix result(left);

    for (size_t i = 0; i < left.getNumberRows(); ++i) {
        for (size_t j = 0; j < left.getNumberCols(); ++j) {
            result.array_[i][j] = std::max(result.array_[i][j], right.array_[i][j]);
        }
    }

    return result;
}
Matrix transpose(const Matrix &matrix) {
    Matrix result(Fraction(), matrix.getNumberCols(), matrix.getNumberRows());

    for (size_t i = 0; i < matrix.getNumberRows(); ++i) {
        for (size_t j = 0; j < matrix.getNumberCols(); ++j) {
            result.array_[j][i] = matrix.array_[i][j];
        }
    }

    return result;
}
Matrix makeRowEchelonForm(const Matrix &matrix) {
    Matrix result(matrix);
    for (size_t col = 0; col < matrix.getNumberRows(); ++col) {
        if (col >= matrix.getNumberCols()) {
            for (size_t i = 0; i < matrix.getNumberCols(); ++i) {
                result.array_[col][i] = 0;
            }
            continue;
        }
        if (col != matrix.getNumberRows() - 1 && result.array_[col][col] == 0) {
            result.shiftRows(col);
        }
        for (size_t row = col + 1; row < matrix.getNumberRows(); ++row) {
            if (result.array_[col][col] == 0) {
                continue;
            }
            Fraction lineCoef = result.array_[row][col] / result.array_[col][col];
            if (lineCoef != 0) {
                for (size_t elem = col; elem < matrix.getNumberCols(); ++elem) {
                    result.array_[row][elem] -= result.array_[col][elem] * lineCoef;
                }
            }
        }
    }
    return result;
}
Fraction minor(Matrix &matrix, const Fraction &row, const Fraction &col) {
    Matrix result(0, matrix.getNumberRows() - 1, matrix.getNumberCols() - 1);
    for (int i = 0, resIndexRow = 0; i < matrix.getNumberRows(); ++i, ++resIndexRow) {
        if (static_cast<Fraction>(i) == row) {
            --resIndexRow;
            continue;
        }
        for (int j = 0, resIndexCol = 0; j < matrix.getNumberCols(); ++j, ++resIndexCol) {
            if (static_cast<Fraction>(j) == col) {
                --resIndexCol;
                continue;
            }
            result[resIndexRow][resIndexCol] = matrix[i][j];
        }
    }

    return det(result);
}
// _________________________________________

// ________MATRIX_CONCATENATION________
Matrix vertcat(const std::vector<Matrix> &matrices) {
    size_t resultRows = 0;
    for (size_t i = 0; i < matrices.size(); ++i) {
        resultRows += matrices[i].getNumberRows();
        if (matrices[i].getNumberCols() != matrices[0].getNumberCols()) {
            throw ConcatenationError();
        }
    }
    Matrix result;
    result.array_ = std::vector<std::vector<Fraction>>(resultRows);

    size_t i = 0;
    for (const auto &matrix: matrices) {
        for (size_t j = 0; j < matrix.getNumberRows(); ++j) {
            result.array_[i++] = matrix.array_[j];
        }
    }
    return result;
}
Matrix horzcat(const std::vector<Matrix> &matrices) {
    size_t resultCols = 0;
    for (const auto &matrix: matrices) {
        resultCols += matrix.getNumberCols();
        if (matrix.getNumberRows() != matrices[0].getNumberRows()) {
            throw ConcatenationError();
        }
    }

    Matrix result;
    result.array_ = std::vector<std::vector<Fraction>>(matrices[0].getNumberRows(),
                                                       std::vector<Fraction>(resultCols, 0));

    for (size_t row = 0; row < matrices[0].getNumberRows(); ++row) {
        size_t col = 0;
        for (const auto &matrix: matrices) {
            for (size_t sub_col = 0; sub_col < matrix.getNumberCols(); ++sub_col) {
                result.array_[row][col++] = matrix.array_[row][sub_col];
            }
        }
    }
    return result;
}
// ____________________________________

// ________OUTPUT________
std::vector<Fraction> &Matrix::operator[](const size_t &index) {
    if (0 <= index && index < getNumberRows()) {
        return array_[index];
    }
    throw OutOfRange();
}

std::ostream &operator<<(std::ostream &out, const Matrix &data) {
    std::vector<size_t> maxLenCol(data.array_[0].size(), 0);
    for (size_t i = 0; i < data.getNumberRows(); ++i) {
        for (size_t j = 0; j < data.getNumberCols(); ++j) {
            maxLenCol[j] = std::max(maxLenCol[j], to_string(data.array_[i][j]).size());
        }
    }

    for (size_t i = 0; i < data.getNumberRows(); ++i) {
        if (i != 0) {
            out << std::endl;
        }
        for (size_t j = 0; j < data.getNumberCols(); ++j) {
            if (j != 0) {
                out << " ";
            }
            out << std::setw((int) (maxLenCol[j])) << data.array_[i][j];
        }
    }
    return out;
}
// ______________________

// _________UTILS_________
size_t Matrix::getNumberRows() const {
    return array_.size();
}
size_t Matrix::getNumberCols() const {
    return array_[0].size();
}
size_t Matrix::getNumberElems() const {
    return getNumberRows() * getNumberCols();
}
void Matrix::shiftRows(const size_t &col) {
    size_t countSwaps = 0;
    for (size_t row = col; row < getNumberRows() - 1; ++row) {
        if (array_[row][col] == 0) {
            bool flag = false;
            size_t row2 = getNumberRows() - 1;
            for (; row2 > row; --row2) {
                if (array_[row2][col] != 0) {
                    flag = true;
                    break;
                }
            }
            if (flag) {
                std::swap(array_[row], array_[row2]);
                ++countSwaps;
            } else {
                break;
            }
        }
    }
    if (countSwaps % 2 != 0) {
        for (auto &elem: array_[0]) {
            elem = -elem;
        }
    }
}
// _______________________