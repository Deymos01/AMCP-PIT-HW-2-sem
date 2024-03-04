#include "validator.h"
#include <cstring>

bool writeDataTimeOrDate(const char *data, int &index, const int &indexUntil, int &val, const char &nextSymbol) {
    while (index < indexUntil && data[index] != '.') {
        if (data[index] < '0' || data[index] > '9') {
            return false;
        }
        val = val * 10 + data[index++] - '0';
    }
    return data[index++] == nextSymbol;
}

// _________________________Checking_date____________________________________

bool isFebruaryError(const int &day, const int &month, const int &year) {
    return month == 2 && (day > (28 + static_cast<int>((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))));
}

void checkInputDate(const char *date, int &day, int &month, int &year) {
    if (strlen(date) > 10) {
        throw 0;
    }
    int index = 0;
    if (!writeDataTimeOrDate(date, index, 2, day, '.') ||
        !writeDataTimeOrDate(date, index, 5, month, '.') ||
        !writeDataTimeOrDate(date, index, 10, year, '\0')) {
        throw 0;
    }

    if (day > 31 || isFebruaryError(day, month, year) ||
        (month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        throw 1;
    }
	if (month > 12) {
        throw 2;
    }
}

// __________________________________________________________________________

void checkInputTime(const char *time, int &hours, int &minutes, int &seconds) {
    if (strlen(time) > 8) {
        throw 0;
    }
    int index = 0;
    if (!writeDataTimeOrDate(time, index, 2, hours, ':') ||
        !writeDataTimeOrDate(time, index, 5, minutes, ':') ||
        !writeDataTimeOrDate(time, index, 8, seconds, '\0')) {
        throw 0;
    }
    if (hours > 23 || minutes > 59 || seconds > 59) {
        throw 1;
    }
}

bool checkInputTicker(char *ticker) {
    for (int i = 0; i < strlen(ticker); ++i){
        if (ticker[i] == ',' || ticker[i] == ';' || ticker[i] == '"'){
            return false;
        }
    }
    return true;
}

bool checkInputType(char *typeOrder) {
    return strcmp(typeOrder, "buy") == 0 || strcmp(typeOrder, "sell") == 0;
}

bool checkInputQuantity(char *quantityStr, int &quantity){
    for (int i = 0; i < strlen(quantityStr); ++i){
        if (quantityStr[i] < '0' || quantityStr[i] > '9'){
            return false;
        }
        quantity = quantity * 10 + quantityStr[i] - '0';
    }
    return true;
}

bool checkInputPrice(char *priceStr, double &price){
    int countDots = 0;
    for (int i = 0; i < strlen(priceStr); ++i){
        if (i != 0 && priceStr[i] == '.' && countDots++ > 1){
            return false;
        }
        if ((priceStr[i] < '0' || priceStr[i] > '9') && priceStr[i] != '.'){
            return false;
        }
    }
    price = strtod(priceStr, nullptr);
    return true;
}