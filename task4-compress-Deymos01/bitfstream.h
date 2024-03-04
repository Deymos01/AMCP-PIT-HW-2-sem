#ifndef TASK4_COMPRESS_DEYMOS01_BITFSTREAM_H
#define TASK4_COMPRESS_DEYMOS01_BITFSTREAM_H

#include <fstream>

class ibitfstream : public std::ifstream {
    unsigned char buffer;
    unsigned char bitcount;

public:
    ibitfstream(const char *filename) : std::ifstream(filename, std::ios_base::binary), bitcount(0) {}

    int getbit() {
        if (bitcount == 8) {
            bitcount = 0;
        }
        if (bitcount == 0) {
            buffer = get(); //std::ifstream::get();
        }
        int result = (buffer & (1 << (7 - bitcount))) != 0 ? 1 : 0;
        ++bitcount;
        return result;
    }

    void back(const long long &step = 1) {
        if (step > tellg()) { return; }
        seekg(static_cast<long long>(tellg()) - step, std::ios::beg);
    }

    std::basic_ifstream<char>::int_type show() {
        std::basic_ifstream<char>::int_type ch = get();
        back();
        return ch;
    }

    bool eof() {
        if (bitcount > 0 && bitcount < 8) {
            return false;
        }
        if (peek() != EOF) {
            return false;
        }
        return true;
    }
};

class obitstream : public std::ofstream {
    unsigned char buffer;
    unsigned char bitcount;
public:
    obitstream(const char *filename) : std::ofstream(filename, std::ios_base::binary),
            bitcount(0), buffer(0) {}

    void putbit(bool x) {
        unsigned char ch = static_cast<unsigned char>(x);
        buffer |= (ch << (7 - bitcount));
        ++bitcount;
        if (bitcount == 8) {
            put(buffer); //std::ofstream::put
            buffer = 0;
            bitcount = 0;
        }
    }

    void close() {
        if (bitcount > 0) {
            put(buffer);
        }
        bitcount = 0;
        buffer = 0;
        std::ofstream::close();
    }
};

#endif //TASK4_COMPRESS_DEYMOS01_BITFSTREAM_H
