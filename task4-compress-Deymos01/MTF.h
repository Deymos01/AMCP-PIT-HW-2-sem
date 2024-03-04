#ifndef TASK4_COMPRESS_MTF_H
#define TASK4_COMPRESS_MTF_H

#include <cstring>
#include "bitfstream.h"

class MTF {
private:
    unsigned char arraySymbol[256]; // This array uses character positions as indexes and character codes as values
    size_t arrayPosSymbol[256]; // This array uses character codes as indexes and values as position

    void updateArrays(const unsigned char &ch) {
        /*
         * Updating supporting arrays: shift array with Symbols and update positions in other array
         */

        size_t temp = arrayPosSymbol[ch];
        arrayPosSymbol[ch] = 0;
        for (int i = (int) (temp) - 1; i >= 0; --i) {
            ++arrayPosSymbol[arraySymbol[i]];
            std::swap(arraySymbol[i + 1], arraySymbol[i]);
        }
    }

public:
    MTF() {
        for (size_t i = 0; i < 256; ++i) {
            arraySymbol[i] = arrayPosSymbol[i] = i;
        }
    }

    void encode(ibitfstream &inFile, obitstream &outFile) {
        unsigned char currChar;

        while (static_cast<char>(currChar = inFile.get()) != EOF) {
            outFile.put(static_cast<char>(arrayPosSymbol[currChar]));
            updateArrays(currChar);
        }
    }

    void decode(ibitfstream &inFile, obitstream &outFile) {
        unsigned char currChar;

        while (static_cast<char>(currChar = inFile.get()) != EOF) {
            outFile.put(static_cast<char>(arraySymbol[currChar]));
            updateArrays(arraySymbol[currChar]);
        }
    }

};

#endif //TASK4_COMPRESS_MTF_H