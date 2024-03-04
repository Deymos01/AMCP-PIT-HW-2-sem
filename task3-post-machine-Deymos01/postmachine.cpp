#include "postmachine.h"

#include <cstring>
#include <fstream>

#include "errors.h"

void loadProgram(std::ifstream &programFile, BinTree *program) {
    while (programFile.peek() != EOF) {
        // Skip commentaries
        if (programFile.peek() == '%') {
            for (; programFile.get() != '\n';) {}
            continue;
        }

        // If the first symbol in current line is digit (current line is command)
        if (isdigit(programFile.peek()) != 0) {
            int lineIndex;
            programFile >> lineIndex;

            if (programFile.get() != '.') {
                throw ErrorSyntaxProgram();
            }
            if (programFile.get() != ' ') {
                throw ErrorSyntaxProgram();
            }

            char commandType = programFile.get();
            int nextIndex1 = 0;
            int nextIndex2 = 0;

            if (commandType != '!') {
                if (programFile.get() != ' ') {
                    throw ErrorSyntaxProgram();
                }
                if (isdigit(programFile.peek()) == 0) {
                    throw ErrorSyntaxProgram();
                }
                programFile >> nextIndex1;

                if (commandType == '?') {
                    if (programFile.get() != ' ') {
                        throw ErrorSyntaxProgram();
                    }
                    if (isdigit(programFile.peek()) == 0) {
                        throw ErrorSyntaxProgram();
                    }
                    programFile >> nextIndex2;
                }
            }

            // If there are spaces at the end of command line, then skip it
            while (programFile.peek() == ' ') {
                programFile.get();
            }

            switch (commandType) {
                case 'V':
                    program->insert(new SetMark(lineIndex, nextIndex1));
                    break;
                case 'X':
                    program->insert(new DelMark(lineIndex, nextIndex1));
                    break;
                case 'L':
                    program->insert(new MoveLeft(lineIndex, nextIndex1));
                    break;
                case 'R':
                    program->insert(new MoveRight(lineIndex, nextIndex1));
                    break;
                case '?':
                    program->insert(new NotMark(lineIndex, nextIndex1, nextIndex2));
                    break;
                case '!':
                    program->insert(new Stop(lineIndex));
                    break;
                default:
                    throw ErrorSyntaxProgram();
            }
            if (programFile.peek() != '%') {
                programFile.ignore(); // skip '\n' at the end of the line
            }
        } else {
            throw ErrorSyntaxProgram();
        }
        if (programFile.peek() == '\n') {
            programFile.ignore();
        }
    }
    program->balance();
}

std::string PostMachine::calc(std::string const &init, int maxIter) {
    // Create a tape of Post Machine
    Tape pmTape{};
    pmTape.len = length_;
    pmTape.tape = new char[length_ + 1];
    memset(pmTape.tape, '0', length_);
    pmTape.tape[length_] = '\0';
    for (int i = 0; init[i] != '\0'; ++i) {
        pmTape.tape[i] = init[i];
    }
    pmTape.position = 0;

    int nextCommandIndex = program->find_minimum()->data->currCommandIndex;

    for (int i = 0; i < maxIter; ++i) {
        try {
            if (program->find(nextCommandIndex) == nullptr) {
                throw ErrorSyntaxProgram();
            }
            nextCommandIndex = program->find(nextCommandIndex)->data->run(pmTape.tape, pmTape.position, length_);
        } catch (QuitProgram q) {
            int lastIndex = pmTape.position;
            int countZeroes = 0;
            int countOnes = 0;

            std::string result(1, pmTape.tape[pmTape.position]);
            ++pmTape.position;
            if (pmTape.position == pmTape.len) {
                pmTape.position = 0;
            }

            while (pmTape.position != lastIndex) {
                if (pmTape.tape[pmTape.position] == '0') {
                    if (countOnes != 0) {
                        result += std::string(countOnes, '1');
                        countOnes = 0;
                    }
                    ++countZeroes;
                } else {
                    if (countZeroes != 0) {
                        result += std::string(countZeroes, '0');
                        countZeroes = 0;
                    }
                    ++countOnes;
                }

                if (++pmTape.position == pmTape.len) {
                    pmTape.position = 0;
                }
            }

            if (countOnes != 0) {
                result += std::string(countOnes, '1');
            }
            return result;
        } catch (...) {
            throw;
        }
    }

    throw ErrorOutTimeLimit();
}