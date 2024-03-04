#include "biginteger.h"
#include <string>
#include <cstring>
#include <stdexcept>

typedef unsigned long long ull;

// String constructor
BigInteger::BigInteger(std::string number) : sign_(true) {
    int length = static_cast<int>(number.length());
    // Checking errors
    if (length != 0) {
        if (number[0] == '-') {
            sign_ = false;
            number.erase(0, 1);
            --length;
        } else if (number[0] == '+') {
            number.erase(0, 1);
            --length;
        }
    }
    if (length == 0) {
        throw std::invalid_argument("Expected non-empty str that represent an integer number");
    }
    for (char elem: number) {
        if (isdigit(elem) == 0) {
            throw std::invalid_argument("Expected number without other symbols");
        }
    }
    // ________________
    int maxDegreeOfTwo = length / 3 * 10 + ((length % 3 == 0) ? -1 : (length % 3) * 3);
    arraySize_ = (maxDegreeOfTwo - 1) / 63 + 1;
    array_ = new ull[arraySize_];
    numberSize_ = arraySize_;
    memset(array_, 0, arraySize_ * sizeof(ull));
    // Writing long number in memory
    int posBinary = 0;
    int currChunk = 0;
    while (length > 0) {
        ull currBit = static_cast<ull>((number.back() - '0') % 2) << posBinary++;
        array_[currChunk] |= currBit;
        if (posBinary > 63) {
            ++currChunk;
            posBinary = 0;
        }
        int remainder = 0;
        for (char &i: number) {
            remainder = remainder * 10 + (i - '0');
            i = static_cast<char>((remainder / 2) + '0');
            remainder %= 2;
        }
        while (length > 0 && number[0] == '0') {
            number.erase(0, 1);
            --length;
        }
    }
    updateNumberSize();
}

// ________COMPARISON_OPERATORS____________
bool operator==(const BigInteger &left, const BigInteger &right) {
    if (left.getArraySize() < right.getArraySize()) {
        return right == left;
    }
    if (left.getSign() != right.getSign() && left.getNumberSize() == left.getNumberSize() &&
        left[0] == 0 && right[0] == 0) {
        return true;
    }
    if (left.getSign() != right.getSign()) {
        return false;
    }
    if (left.getNumberSize() != right.getNumberSize()) {
        return false;
    }
    // Checking chunks equal
    int i = 0;
    for (; i < right.getArraySize(); ++i) {
        if (left[i] != right[i]) {
            return false;
        }
    }
    for (; i < left.getArraySize(); ++i) {
        if (left[i] != 0) {
            return false;
        }
    }
    return true;
}
bool operator!=(const BigInteger &left, const BigInteger &right) {
    return !(left == right);
}
bool operator<(const BigInteger &left, const BigInteger &right) {
    if (left.getSign() != right.getSign() && left[0] == 0 && right[0] == 0 || left == right) {
        return false;
    }
    if (left.getNumberSize() < right.getNumberSize()) {
        return true;
    }
    if (!left.getSign() && right.getSign()) {
        return true;
    }
    if (left.getSign() && !right.getSign()) {
        return false;
    }
    // left.sign == right.sign
    if (left.getNumberSize() > right.getNumberSize()) {
        return false;
    }
    // left.numberSize == right.numberSize
    if (!left.getSign()) {
        return -right < -left;
    }
    for (int i = (int) (left.getNumberSize() - 1); i >= 0; --i) {
        if (left[i] != right[i]) {
            return left[i] < right[i];
        }
    }
    return true;
}
bool operator<=(const BigInteger &left, const BigInteger &right) {
    return left < right || left == right;
}
bool operator>(const BigInteger &left, const BigInteger &right) {
    return right < left;
}
bool operator>=(const BigInteger &left, const BigInteger &right) {
    return left > right || left == right;
}

// ________________________________________
// ________ARITHMETICS_OPERATORS___________
BigInteger operator+(const BigInteger &left, const BigInteger &right) {
    BigInteger tmp(left);
    tmp += right;
    return tmp;
}
BigInteger &BigInteger::operator+=(const BigInteger &other) {
    if (!getSign() && other.getSign()) {
        *this = other - (-(*this));
        return *this;
    }
    if (getSign() && !other.getSign()) {
        *this = (*this) - (-other);
        return *this;
    }
    if (getNumberSize() > other.getNumberSize()) {
        *this = other + *this;
        return *this;
    }

    ull addBlocks = other.getNumberSize() - getNumberSize();
    if (addBlocks > 0) {
        expandArray(addBlocks);
    }

    ull buff = 0ULL;
    int i = 0;
    for (; i < getNumberSize(); ++i) {
        (*this)[i] = (*this)[i] + other[i] + buff;
        buff = ((*this)[i] < other[i]) ? 1ULL : 0ULL;
    }
    for (; i < other.getNumberSize(); ++i) {
        (*this)[i] = other[i] + buff;
        buff = ((*this)[i] < other[i]) ? 1ULL : 0ULL;
    }
    if (buff != 0ULL) {
        expandArray();
        (*this)[i] = buff;
    }
    updateNumberSize();
    return *this;
}
BigInteger &BigInteger::operator++() {
    if (getNumberSize() == 1 && array_[0] == 0ULL) {
        sign_ = true;
        ++array_[0];
        return *this;
    }

    int index = 0;
    if (getSign()) {
        while (index < arraySize_ && (++array_[index] == 0ULL)) {
            ++index;
        }
        if (index != 0 && array_[index - 1] == 0ULL) {
            expandArray();
            ++array_[index];
            ++numberSize_;
        }
    } else {
        sign_ = true;
        --(*this);
        sign_ = false;
    }
    return *this;
}
BigInteger BigInteger::operator++(int) {
    BigInteger tmp = *this;
    ++(*this);
    return tmp;
}
BigInteger operator-(const BigInteger &left, const BigInteger &right) {
    BigInteger tmp(left);
    tmp -= right;
    return tmp;
}
BigInteger &BigInteger::operator-=(const BigInteger &other) {
    if (getSign() && !other.getSign() || !getSign() && other.getSign()) {
        *this = *this + (-other);
        return *this;
    }
    if (!getSign() && !other.getSign()) {
        *this = (-other) - (-(*this));
        return *this;
    }
    if ((*this) < other) {
        *this = -getAbsDifference(other, *this);
    } else {
        *this = getAbsDifference(*this, other);
    }
    return *this;
}
BigInteger &BigInteger::operator--() {
    if (getNumberSize() == 1 && array_[0] == 0ULL) {
        sign_ = false;
        ++array_[0];
        return *this;
    }

    int index = 0;
    if (getSign()) {
        while (index < arraySize_ && (--array_[index] == UINT64_MAX)) {
            ++index;
        }
    } else {
        sign_ = true;
        ++(*this);
        sign_ = false;
    }
    updateNumberSize();
    return *this;
}
BigInteger BigInteger::operator--(int) {
    BigInteger tmp = *this;
    --(*this);
    return tmp;
}
BigInteger operator*(const BigInteger &left, const BigInteger &right) {
    BigInteger tmp(left);
    tmp *= right;
    return tmp;
}
BigInteger &BigInteger::operator*=(const BigInteger &other) {
    auto *leftArray = reinterpret_cast<uint32_t *>(getArray());
    auto *rightArray = reinterpret_cast<uint32_t *>(other.getArray());
    BigInteger result = 0;
    result.setSign(getSign() == other.getSign());
    result.expandArray(static_cast<ull>(getNumberSize() + other.getNumberSize() - 1));
    auto *resultArray = reinterpret_cast<uint32_t *>(result.getArray());
    ull base = (ull) UINT32_MAX + 1;
    for (int i = 0; i < other.getNumberSize() * 2; ++i) {
        ull carry = 0ULL;
        for (int j = 0; j < getNumberSize() * 2; ++j) {
            ull tmp = static_cast<ull>(leftArray[j]) * rightArray[i] + carry + resultArray[i + j];
            resultArray[i + j] = tmp % base;
            carry = tmp / base;
        }
        resultArray[i + getNumberSize() * 2] += carry;
    }
    result.updateNumberSize();
    *this = result;
    return *this;
}
BigInteger operator/(const BigInteger &left, const BigInteger &right) {
    BigInteger tmp(left);
    tmp /= right;
    return tmp;
}
BigInteger &BigInteger::operator/=(const BigInteger &other) {
    bool resSign = sign_ == other.sign_;
    sign_ = true;
    BigInteger other_abs(other);
    other_abs.sign_ = true;

    if (*this < other_abs) {
        *this = 0;
        return *this;
    }

    if (other_abs == BigInteger(1)) {
        (*this).sign_ = resSign;
        return *this;
    }

    BigInteger divisor(other);
    BigInteger remainder(*this);
    divisor.sign_ = remainder.sign_ = true;

    while (divisor <= *this) {
        divisor *= 2;
    }

    memset(array_, 0, arraySize_ * sizeof(ull));
    while (divisor >= other_abs) {
        *this *= 2;
        if (remainder >= divisor) {
            remainder -= divisor;
            array_[0] |= 1;
        }
        divideByTwo(divisor);
    }

    sign_ = resSign;
    updateNumberSize();
    return *this;
}
BigInteger operator%(const BigInteger &left, const BigInteger &right) {
    BigInteger tmp(left);
    tmp %= right;
    return tmp;
}
BigInteger &BigInteger::operator%=(const BigInteger &other) {
    *this = *this - other * (*this / other);
    if (!(*this).getSign() || other.getSign()) {
        setSign(getSign() == other.getSign());
    }
    updateNumberSize();
    return *this;
}
BigInteger::operator size_t() const {
    return array_[0];
}
// ________________________________________
std::string to_string(const BigInteger &obj) {
    std::string result = "0";
    for (int i = (int) (obj.getArraySize() - 1); i >= 0; --i) {
        ull temp = obj[i];
        ull pointer = (1ULL << (sizeof(ull) * 8 - 1));
        int bit = 63;
        while (i == obj.getArraySize() - 1 && ((pointer & temp) == 0) && bit >= 0) {
            pointer >>= 1;
            --bit;
        }
        while (bit >= 0) {
            multiplyByTwo(result);
            if ((pointer & temp) != 0) {
                plusOne(result);
            }
            pointer >>= 1;
            --bit;
        }
    }
    if (!obj.getSign() && result != "0") {
        result.insert(0, 1, '-');
    }
    return result;
}

// _________Utils__________
BigInteger getAbsDifference(const BigInteger &left, const BigInteger &right) {
    if (left < right) {
        return getAbsDifference(right, left);
    }
    BigInteger result = left;
    ull tmp;
    ull buff = 0ULL;
    int i = 0;
    for (; i < right.getNumberSize(); ++i) {
        if (left[i] < right[i]) {
            tmp = UINT64_MAX - right[i] + left[i] + 1 - buff;
            buff = 1ULL;
        } else {
            tmp = left[i] - right[i] - buff;
            if (left[i] != 0ULL) {
                buff = 0ULL;
            }
        }
        result[i] = tmp;
    }
    if (buff != 0) {
        for (; i < left.getNumberSize(); ++i) {
            if (result[i] == 0) {
                result[i] = UINT64_MAX - 1;
            } else {
                --result[i];
                break;
            }
        }
    }
    result.updateNumberSize();
    return result;
}

void multiplyByTwo(std::string &str) {
    int buff = 0;
    for (int i = (int) (str.length() - 1); i >= 0; --i) {
        int result = (str[i] - '0') * 2 + buff;
        str[i] = static_cast<char>(result % 10 + '0');
        buff = result / 10;
    }
    if (buff != 0) {
        str.insert(0, 1, static_cast<char>(buff + '0'));
    }
}

void plusOne(std::string &str) {
    int buff = 1;
    for (int i = (int) (str.length() - 1); i >= 0; --i) {
        int result = (str[i] - '0') + buff;
        str[i] = static_cast<char>(result % 10 + '0');
        buff = result / 10;
    }
    if (buff != 0) {
        str.insert(0, 1, static_cast<char>(buff + '0'));
    }
}

void divideByTwo(BigInteger &obj) {
    for (int i = 0; i < obj.getNumberSize(); ++i) {
        if (i != 0 && (obj[i] & 1ULL) != 0U) {
            obj[i - 1] |= (1ULL << 63);
        }
        obj[i] >>= 1;
    }
    obj.updateNumberSize();
}

BigInteger getGCD(BigInteger left, BigInteger right) {
    left.setSign(true);
    right.setSign(true);

    while (left != BigInteger(0) && right != BigInteger(0)) {
        if (left > right) {
            left %= right;
        } else {
            right %= left;
        }
    }

    return left + right;
}

// __________________________