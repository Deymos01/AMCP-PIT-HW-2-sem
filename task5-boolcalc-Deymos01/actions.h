#ifndef TASK5_BOOLCALC_DEYMOS01_ACTIONS_H
#define TASK5_BOOLCALC_DEYMOS01_ACTIONS_H

const unsigned char ActionsTable[][13] = {
        //      \0      ~      &      v      +      ->     <-     =      |       ^      (       )       x
        {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 7, 1}, // \0
        {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 3, 1}, // ~
        {3, 2, 3, 3, 3, 3, 3, 3, 3, 3, 2, 3, 1}, // &
        {3, 2, 2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 1}, // v
        {3, 2, 2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 1}, // +
        {3, 2, 2, 2, 2, 3, 3, 3, 2, 2, 2, 3, 1}, // ->
        {3, 2, 2, 2, 2, 3, 3, 3, 2, 2, 2, 3, 1}, // <-
        {3, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 3, 1}, // =
        {3, 2, 2, 2, 2, 3, 3, 3, 3, 3, 2, 3, 1}, // |
        {3, 2, 2, 2, 2, 3, 3, 3, 2, 3, 2, 3, 1}, // ^
        {6, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 5, 1} // (
};

inline int actionRowNumber(const char &ch) {
    switch (ch) {
        case 0:
            return 0;
        case '~':
            return 1;
        case '&':
            return 2;
        case 'v':
            return 3;
        case '+':
            return 4;
        case '>':
            return 5;
        case '<':
            return 6;
        case '=':
            return 7;
        case '|':
            return 8;
        case '^':
            return 9;
        case '(':
            return 10;
    }
    throw "Unexpected symbol";
}

inline int actionColNumber(const char &ch) {
    switch (ch) {
        case 0:
            return 0;
        case '~':
            return 1;
        case '&':
            return 2;
        case 'v':
            return 3;
        case '+':
            return 4;
        case '>':
            return 5;
        case '<':
            return 6;
        case '=':
            return 7;
        case '|':
            return 8;
        case '^':
            return 9;
        case '(':
            return 10;
        case ')':
            return 11;
        case 'x':
            return 12;
    }

    if (ch == 't' || ch == 'f') {
        return 12;
    }
    throw "Unexpected symbol";
}

#endif //TASK5_BOOLCALC_DEYMOS01_ACTIONS_H
