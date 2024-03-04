#ifndef TASK4_COMPRESS_HUFFMAN_H
#define TASK4_COMPRESS_HUFFMAN_H

#include <cstring>
#include "node.h"
#include "list.h"
#include "bitfstream.h"

class Huffman {
private:
    struct CharCodeMap {
        int lenCode = 0;
        unsigned long long code = 0;
        unsigned char symbol = 0;
    };

    PriorityList freeNodes;
    CharCodeMap *charTable;
    Node *fanoTree;
    int *frequencyTable;
    unsigned int numberUniqChars;

    void fillFrequencyTableEncode(ibitfstream &inFile) {
        unsigned char symbol;
        while (static_cast<char>(symbol = inFile.get()) != EOF) {
            ++frequencyTable[symbol];
        }
    }

    void fillFrequencyTable(ibitfstream &inFile) {
        unsigned char symbol;
        int frequency = 0;
        for (int i = 0; i < numberUniqChars; ++i) {
            symbol = inFile.get();
            inFile.read((char *) (&frequency), sizeof(frequency));
            frequencyTable[symbol] = frequency;
        }
    }

    Node *createFanoTree() {
        // Filling free-nodes list
        for (size_t i = 0; i < 256; ++i) {
            if (frequencyTable[i] != 0) {
                Node *newNode = new Node(static_cast<unsigned char>(i), frequencyTable[i]);
                freeNodes.insert(newNode);
            }
        }

        numberUniqChars = freeNodes.size();
        // Creating Fano-tree
        while (freeNodes.size() != 1) {
            Node *min1 = freeNodes.pop_front();
            Node *min2 = freeNodes.pop_front();
            Node *newNode = new Node('\0', min1->frequency);
            newNode->left = min1;
            newNode->right = min2;
            if (min2 != nullptr) {
                newNode->frequency += min2->frequency;
            }
            freeNodes.insert(newNode);
        }
        return freeNodes.pop_front();
    }

    void fillCodeTable(Node *elem, unsigned long long code = 0, int codeLength = 0) {
        static int index = 0;

        if (elem == nullptr) { return; }
        if (elem->left == nullptr && elem->right == nullptr) {
            if (codeLength == 0) { ++codeLength; }
            charTable[index].lenCode = codeLength;
            charTable[index].symbol = elem->symbol;
            charTable[index].code = code;
            ++index;
        } else {
            fillCodeTable(elem->left, (code << 1) | 0, ++codeLength);
            --codeLength;
            fillCodeTable(elem->right, (code << 1) | 1, ++codeLength);
        }
    }
public:
    Huffman() : fanoTree(nullptr) {
        numberUniqChars = 0;
        frequencyTable = new int[256]{0};
        charTable = nullptr;
    }
    ~Huffman() {
        delete fanoTree;
        fanoTree = nullptr;
        delete[] frequencyTable;
        delete[] charTable;
    }

    void encode(ibitfstream &inFile, obitstream &outFile) {
        //Collecting statistic
        int pos = inFile.tellg();
        inFile.seekg(0, std::ios::end);
        unsigned long long dataSize = inFile.tellg();
        inFile.seekg(pos, std::ios::beg);

        fillFrequencyTableEncode(inFile);

        fanoTree = createFanoTree();
        charTable = new CharCodeMap[numberUniqChars];
        fillCodeTable(fanoTree);

        outFile.write((const char *) (&dataSize), sizeof(dataSize));
        outFile.write((const char *) (&numberUniqChars), sizeof(numberUniqChars));
        for (int i = 0; i < 256; ++i) {
            if (frequencyTable[i] != 0) {
                outFile << static_cast<unsigned char>(i);
                outFile.write((const char *) (frequencyTable + i), sizeof(frequencyTable[i]));
            }
        }

        char symbol;
        unsigned long long code = 0;
        int codeLength = 0;
        inFile.clear();
        inFile.seekg(pos, std::ios::beg);

        while ((symbol = static_cast<char>(inFile.get())) != EOF) {
            for (int j = 0; j < numberUniqChars; ++j) {
                if (charTable[j].symbol == static_cast<unsigned char>(symbol)) {
                    code = charTable[j].code;
                    codeLength = charTable[j].lenCode;
                    break;
                }
            }

            bool bit;
            for (int j = 0; j < codeLength; ++j) {
                bit = (((code >> (codeLength - 1 - j)) & 1) != 0U);
                outFile.putbit(bit);
            }
        }
    }

    void decode(ibitfstream &inFile, obitstream &outFile) {
        unsigned long long dataSize;
        inFile.read((char *)(&dataSize), sizeof(dataSize));
        inFile.read((char *)(&numberUniqChars), sizeof(numberUniqChars));

        // Filling frequency table
        fillFrequencyTable(inFile);

        fanoTree = createFanoTree();
        bool currBit;
        Node *pointer = fanoTree;
        while (!inFile.eof() && dataSize > 0) {
            while (pointer != nullptr && (pointer->left != nullptr || pointer->right != nullptr)) {
                currBit = (inFile.getbit() != 0);
                if (static_cast<int>(currBit) == 0) {
                    pointer = pointer->left;
                } else {
                    pointer = pointer->right;
                }
            }
            if (pointer == nullptr) { return; }
            outFile << pointer->symbol;
            pointer = fanoTree;
            --dataSize;
        }
    }
};

#endif //TASK4_COMPRESS_HUFFMAN_H