#ifndef BIGINTEGER_H
#define BIGINTEGER_H

typedef unsigned long long ull;

#include <cstring>
#include <stdexcept>
#include <iostream>

class BigInteger {
private:
    ull *array_;
    size_t arraySize_;
    size_t numberSize_;
    bool sign_;  // true - "+", false - "-"

    void swap(BigInteger &other) {
        std::swap(array_, other.array_);
        std::swap(arraySize_, other.arraySize_);
        std::swap(numberSize_, other.numberSize_);
        std::swap(sign_, other.sign_);
    }

    ull *getArray() const { return array_; }

public:
    // _____________Constructors____________
    BigInteger() : BigInteger(static_cast<ull>(0)) {}
    BigInteger(unsigned int n) : BigInteger(static_cast<ull>(n)) {}
    BigInteger(unsigned long n) : BigInteger(static_cast<ull>(n)) {}
    BigInteger(unsigned short n) : BigInteger(static_cast<ull>(n)) {}
    BigInteger(unsigned char n) : BigInteger(static_cast<ull>(n)) {}
    BigInteger(int n) : BigInteger(static_cast<long long>(n)) {}
    BigInteger(long n) : BigInteger(static_cast<long long>(n)) {}
    BigInteger(short n) : BigInteger(static_cast<long long>(n)) {}
    BigInteger(char n) : BigInteger(static_cast<long long>(n)) {}
    BigInteger(long long n) : sign_(n >= 0), arraySize_(1), numberSize_(1), array_(new ull[1]{static_cast<ull>(std::abs(n))}) {}
    BigInteger(ull n) : sign_(true), arraySize_(1), numberSize_(1), array_(new ull[1]{n}) {}
    BigInteger(std::string number);

    // Copy constructor
    BigInteger(const BigInteger &obj) : sign_(obj.sign_), arraySize_(obj.arraySize_), numberSize_(obj.numberSize_) {
        array_ = new ull[arraySize_];
        memcpy(array_, obj.array_, arraySize_ * sizeof(ull));
    }

    // Move constructor
    BigInteger(BigInteger &&obj) noexcept: array_(nullptr) { swap(obj); }
    // ________________________________________

    ~BigInteger() {
        delete[] array_;
        array_ = nullptr;
        arraySize_ = 0;
        numberSize_ = 0;
    }

    // __________Operators__________
    ull &operator[](size_t index) {
        if (index >= arraySize_) {
            throw std::invalid_argument("Out of BigInt limits");
        }
        return array_[index];
    }
    const ull &operator[](size_t index) const {
        if (index >= arraySize_) {
            throw std::invalid_argument("Out of BigInt limits");
        }
        return array_[index];
    }
    BigInteger &operator=(const BigInteger &other) {
        if (this != &other) {
            BigInteger tmp(other);
            swap(tmp);
        }
        return *this;
    }
    BigInteger &operator=(BigInteger &&other) noexcept {
        if (this != &other) {
            swap(other);
        }
        return *this;
    }
    BigInteger operator-() const {
        BigInteger tmp(*this);
        tmp.sign_ = !tmp.sign_;
        return tmp;
    }
    BigInteger operator+() const {
        return *this;
    }
    BigInteger &operator++();
    BigInteger operator++(int);
    BigInteger &operator--();
    BigInteger operator--(int);
    BigInteger &operator+=(const BigInteger &);
    BigInteger &operator-=(const BigInteger &);
    BigInteger &operator*=(const BigInteger &);
    BigInteger &operator/=(const BigInteger &);
    BigInteger &operator%=(const BigInteger &);
    // _____________________________

    // __________Getters____________
    size_t getArraySize() const { return arraySize_; }
    size_t getNumberSize() const { return numberSize_; }
    bool getSign() const { return sign_; }
    // _____________________________

    // ___________Setters____________
    void setSign(bool val) { sign_ = val; }

    // __________Utils______________
    void expandArray(ull addChunk = 1) {
        if (addChunk > 0) {
            arraySize_ += addChunk;
            ull *newArray = new ull[arraySize_];
            memset(newArray, 0, arraySize_ * sizeof(ull));
            memmove(newArray, array_, (arraySize_ - addChunk) * sizeof(ull));
            delete[] array_;
            array_ = newArray;
        }
    }
    void updateNumberSize() {
        numberSize_ = arraySize_;
        while (numberSize_ > 1 && array_[numberSize_ - 1] == 0) {
            --numberSize_;
        }
    }
    friend BigInteger getAbsDifference(const BigInteger &, const BigInteger &);
    friend void divideByTwo(BigInteger &);
    // _____________________________
};

// ________COMPARISON_OPERATORS____________
bool operator==(const BigInteger &, const BigInteger &);
bool operator!=(const BigInteger &, const BigInteger &);
bool operator<(const BigInteger &, const BigInteger &);
bool operator<=(const BigInteger &, const BigInteger &);
bool operator>(const BigInteger &, const BigInteger &);
bool operator>=(const BigInteger &, const BigInteger &);
// ________________________________________

// ________ARITHMETICS_OPERATORS________
BigInteger operator+(const BigInteger &, const BigInteger &);
BigInteger operator-(const BigInteger &, const BigInteger &);
BigInteger operator*(const BigInteger &, const BigInteger &);
BigInteger operator/(const BigInteger &, const BigInteger &);
BigInteger operator%(const BigInteger &, const BigInteger &);
// _____________________________________

std::string to_string(const BigInteger &);

// _____________Utils_______________
void multiplyByTwo(std::string &);
void plusOne(std::string &);
// _________________________________

std::ostream &operator<<(std::ostream &out, const BigInteger &obj);

#endif  // BIGINTEGER_H