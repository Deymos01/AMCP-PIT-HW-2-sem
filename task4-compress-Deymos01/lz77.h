#ifndef TASK4_COMPRESS_DEYMOS01_LZ77_H
#define TASK4_COMPRESS_DEYMOS01_LZ77_H

#include <cstring>
#include <cstdint>
#include "bitfstream.h"

const unsigned char MAX_VAL = UINT8_MAX;
const long long MAX_BUFFER_SIZE = 5;

class LZ77 {
private:
    struct code {
        unsigned char length = 0;
        unsigned char offset = 0;
        unsigned char next = MAX_VAL;
    };

    long long bufferBeg;
    long long currBufferSize;
    long long maxBufferSize;
    unsigned char *BUFFER;

    void updateBuffer(unsigned int &begBuffer, int shift = 1) {
        if (shift == 0) { shift = 1; }
        if (currBufferSize + shift <= maxBufferSize) {
            currBufferSize += shift;
        } else {
            begBuffer += shift - (maxBufferSize - currBufferSize);
            currBufferSize = maxBufferSize;
        }
    }

    void shiftBufferStr(unsigned char *buffer, size_t lengthAddStr, const unsigned char *add) {
        if (currBufferSize + lengthAddStr >= maxBufferSize) {
            if (lengthAddStr >= maxBufferSize) {
                // We can simply write extra line into the buffer over old data
                for (size_t i = 0, addIndex = lengthAddStr - maxBufferSize; i < maxBufferSize; ++i, ++addIndex) {
                    buffer[i] = add[addIndex];
                }
            } else {
                size_t shift = lengthAddStr - (maxBufferSize - currBufferSize);
                // Offset of elements in buffer
                for (int i = 0; shift + i < currBufferSize; ++i) {
                    buffer[i] = buffer[shift + i];
                }
                // Writing a string to the buffer
                for (unsigned long i = currBufferSize - shift, addIndex = 0; addIndex < lengthAddStr; ++i, ++addIndex) {
                    buffer[i] = add[addIndex];
                }
            }
            currBufferSize = maxBufferSize;
        } else {
            // We can add extra line to the end of the buffer, there will be no overflow
            for (auto i = 0; i < lengthAddStr; ++i, ++currBufferSize) {
                buffer[currBufferSize] = add[i];
            }
        }
    }

public:
    LZ77() : bufferBeg(0), currBufferSize(0), maxBufferSize(MAX_BUFFER_SIZE) {
        BUFFER = new unsigned char[maxBufferSize];
        memset(BUFFER, 0, maxBufferSize * sizeof(unsigned char));
    }

    ~LZ77() {
        delete[] BUFFER;
    }

    void encode(ibitfstream &inFile, obitstream &outFile) {

        while (inFile.show() != EOF) {
            code maxSubstr{0,0,static_cast<unsigned char>(inFile.get())};
            if (currBufferSize == 0) {
                ++currBufferSize;
                BUFFER[bufferBeg++] = maxSubstr.next;
                outFile << "\0\0" << maxSubstr.next;
                continue;
            }

            // рассматриваем все варианты начала подстроки в буфере
            for (size_t offset = 1; offset <= currBufferSize && maxSubstr.length != 255; ++offset) {
                code currSubstr = maxSubstr;
                unsigned char chInBuffer;
                if (currBufferSize == maxBufferSize) {
                    size_t index = (bufferBeg - offset) % currBufferSize;
                    chInBuffer = BUFFER[index];
                } else {
                    chInBuffer = BUFFER[currBufferSize - offset];
                }

                unsigned char chInFile = currSubstr.next;
                long long filePointer = inFile.tellg();

                while(chInBuffer == chInFile) { // Пока текущий символ в файле совпадает с символом в кольцевом буфере
                    ++currSubstr.length;
                    currSubstr.offset = offset;
                    if (inFile.peek() == EOF) {
                        currSubstr.next = 255;
                        break;
                    }
                    chInFile = inFile.get();

                    // TODO: Надо подумать, как просчитать индекс в буфере

                }
                // Возвращаем файловый указатель в исходное положение
                inFile.back(inFile.tellg() - filePointer);
            }
        }

    }

    void decode(ibitfstream &inFile, obitstream &outFile) {
        unsigned char *buffer = new unsigned char[maxBufferSize + 1];
        memset(buffer, 0, maxBufferSize + 1);

        while (inFile.peek() != EOF) {
            code token{0, 0, 0};
            token.offset = inFile.get();
            token.length = inFile.get();
            token.next = inFile.get();

            unsigned char *last = new unsigned char[1];
            last[0] = token.next;

            if (token.offset == 0) {
                if (currBufferSize == maxBufferSize) {
                    shiftBufferStr(buffer, 1, last);
                } else {
                    buffer[currBufferSize++] = token.next;
                }
            } else {
                unsigned int bufferPointerCopy = currBufferSize - token.offset;
                unsigned char *substr = new unsigned char[token.length + 1];

                for (int i = 0; i < token.length; ++i) {
                    unsigned char outCh = buffer[bufferPointerCopy++];
                    substr[i] = outCh;
                    if (bufferPointerCopy == currBufferSize) {
                        bufferPointerCopy = 0;
                    }
                    outFile << outCh;
                }
                substr[token.length] = last[0];

                shiftBufferStr(buffer, token.length + 1, substr);

                delete[] substr;
            }

            if (inFile.peek() != EOF || token.next != 255) {
                outFile << token.next;
            }
            delete[] last;
        }
        delete[] buffer;
    }
};

#endif //TASK4_COMPRESS_DEYMOS01_LZ77_H