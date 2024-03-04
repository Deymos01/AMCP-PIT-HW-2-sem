#include <iostream>
#include <cstring>
#include "MTF.h"
#include "huffman.h"
#include "lz77.h"
#include "bitfstream.h"

void printInfo();

int main(int argc, char *argv[]) {
    if (argc < 2 || argc == 3) {
        std::cerr << "Not enough arguments\n";
        return -1;
    }

    if (argc == 2) {
        if (strcmp(argv[1], "?") == 0 || strcmp(argv[1], "-h") == 0) {
            // Output helpful information about this program
            printInfo();
            return 0;
        }
    }

    MTF mtf;
    Huffman huff;
    LZ77 lz;

    char algorithms = '\0';

    if (argc == 4 && strcmp(argv[1], "-d") == 0) {
        // Decompressing file
        ibitfstream inFile(argv[argc - 2]);
        if (!inFile) {
            std::cerr << "Decompressing:\nCan't open file: " << argv[argc - 2] << "\n";
            return 3;
        }

        inFile.get(algorithms);

        obitstream tempFile("temp");
        obitstream outFile(argv[argc - 1]);
        if (!outFile) {
            std::cerr << "Decompressing:\nCan't open file\n";
            return 3;
        }

        if ((algorithms & 4) == 4) {
            // Decompress Huffman
            huff.decode(inFile, tempFile); // Decompress Huffman
            inFile.close();
            tempFile.close();
            inFile.open("temp", std::ios::binary);
            outFile << inFile.rdbuf(); // Copy data from temp file to output file
            inFile.close();
            outFile.close();
            inFile.open(argv[argc - 1], std::ios::binary);
            tempFile.open("temp", std::ios::binary);
        }
        if ((algorithms & 2) == 2) {
            // Decompress LZ77
            lz.decode(inFile, tempFile); // Decompress LZ77
            inFile.close();
            tempFile.close();
            outFile.close();
            inFile.open("temp", std::ios::binary);
            outFile.open(argv[argc - 1], std::ios::binary);
            outFile << inFile.rdbuf(); // Copy data from temp file to output file
            inFile.close();
            outFile.close();
            inFile.open(argv[argc - 1], std::ios::binary);
            tempFile.open("temp", std::ios::binary);
        }
        if ((algorithms & 1) == 1) {
            // Decompress MTF
            mtf.decode(inFile, tempFile); // Decompress MTF
            inFile.close();
            tempFile.close();
            outFile.close();
            inFile.open("temp", std::ios::binary);
            outFile.open(argv[argc - 1], std::ios::binary);
            outFile << inFile.rdbuf(); // Copy data from temp file to output file
        }

        inFile.close();
        outFile.close();
        tempFile.close();

        std::remove("temp");
    } else {
        // Compress file
        ibitfstream inFile(argv[argc - 2]);

        if (!inFile) {
            inFile.close();
            std::cerr << "Can't open file: \"" << argv[argc - 2] << "\"\n";
            return -3;
        }


        for (int i = 1; i < argc - 2; ++i) {
            if (strcmp(argv[i], "-mtf") == 0) {
                algorithms |= 1;
            } else if (strcmp(argv[i], "-lz77") == 0) {
                algorithms |= 2;
            } else if (strcmp(argv[i], "-huffman") == 0) {
                algorithms |= 4;
            } else {
                std::cerr << "Entered wrong arguments" << '\n';
                return -2;
            }
        }

        obitstream outFile(argv[argc - 1]);
        if (!outFile) {
            inFile.close();
            std::cerr << "Can't open file: \"" << argv[argc - 1] << "\"\n";
            return -3;
        }

        if ((algorithms & 1) == 1) {
            // Algorithm Move-To-Front. Encoding.
            outFile << algorithms;
            mtf.encode(inFile, outFile);
            inFile.close();
            outFile.close();
        }
        if ((algorithms & 2) == 2) {
            // Algorithm LZ77. Encoding.
            if ((algorithms & 1) == 1) {
                // If MTF algorithm is used, then we need to read data from output file
                inFile.open(argv[argc - 1], std::ios::binary);
                inFile.ignore(); // Ignore first byte with code algorithm
                outFile.open(argv[argc - 1], std::ios::binary);
            }
            outFile << algorithms;
            lz.encode(inFile, outFile);
            inFile.close();
            outFile.close();
        }
        if ((algorithms & 4) == 4) {
            // Huffman's algorithm. Encoding.
            if (algorithms != 4) {
                // If MTF or LZ77 algorithm is used, then we need to read data from output file
                inFile.open(argv[argc - 1], std::ios::binary);
                inFile.ignore(); // Ignore first byte with code algorithm
                outFile.open(argv[argc - 1], std::ios::binary);
            }
            outFile << algorithms;
            huff.encode(inFile, outFile);
            inFile.close();
            outFile.close();
        }
    }
    return 0;
}

void printInfo() {
    std::cout << "This program offers you 2 compression algorithms (Huffman and LZ77) and a Move-To-Front conversion.\n"
                 "To use this program, please enter the name of the program, names of the algorithms, \n"
                 "the name of input and output file in the command line. \n\n"
                 "Commands:\n"
                 "-mtf\t\tApplying the MTF algorithm to the input file\n"
                 "-lz77\t\tApplying the LZ77 compression algorithm to the input file\n"
                 "-huffman\tApplying the Huffman's compression algorithm to the input file\n"
                 "-d\t\tDecompressing the input file\n"
                 "-h or ?\t\tOutput of reference information\n\n"
                 "Examples of calling the archiver:\n"
                 "compress -mtf input output\n"
                 "compress -lz77 input output\n"
                 "compress -huffman input output\n"
                 "compress -lz77 -huffman input output\n"
                 "compress -lz77 -mtf -huffman input output\n"
                 "compress -d input output\n"
                 "compress -h\n\n";
}