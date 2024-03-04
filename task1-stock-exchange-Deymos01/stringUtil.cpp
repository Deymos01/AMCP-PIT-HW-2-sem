#include <iostream>
#include <cstring>
#include "stringUtil.h"

void ignoreSpace() {
    while (std::cin.peek() == ' ') {
        std::cin.ignore();
    }
}

bool compareStrings(char *str1, char *str2, bool ignoreRegister = false) {
    if (strlen(str1) != strlen(str2)) {
        return false;
    }
    int index = 0;

    while (str1[index] != 0) {
        if (ignoreRegister && (tolower(str1[index]) != tolower(str2[index]))) {
            return false;
        }
		if (!ignoreRegister && (str1[index] != str2[index])) {
            return false;
        }
        ++index;
    }

    return true;
}
