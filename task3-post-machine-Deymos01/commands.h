#ifndef TASK3_POST_MACHINE_DEYMOS01_COMMANDS_H
#define TASK3_POST_MACHINE_DEYMOS01_COMMANDS_H

#include "errors.h"

class Command {
protected:
    int nextCommandIndex1;
    int nextCommandIndex2;

public:
    int currCommandIndex;

    Command(int index) : currCommandIndex(index) {
        nextCommandIndex1 = nextCommandIndex2 = 0;
    }

    virtual int run(char *str, int &currPosition, int size) {
        return 0;
    }
};

class SetMark : public Command {
public:
    SetMark(int index, int nextIndex1) : Command(index) {
        nextCommandIndex1 = nextIndex1;
    }

    int run(char *str, int &currPosition, int size) {
        str[currPosition] = '1';
        return this->nextCommandIndex1;
    }
};

class DelMark : public Command {
public:
    DelMark(int index, int nextIndex1) : Command(index) {
        nextCommandIndex1 = nextIndex1;
    }

    int run(char *str, int &currPosition, int size) {
        str[currPosition] = '0';
        return this->nextCommandIndex1;
    }
};

class MoveLeft : public Command {
public:
    MoveLeft(int index, int nextIndex1) : Command(index) {
        nextCommandIndex1 = nextIndex1;
    }

    int run(char *str, int &currPosition, int size) {
        --currPosition;
        if (currPosition < 0) {
            currPosition = size - 1;
        }
        return this->nextCommandIndex1;
    }
};

class MoveRight : public Command {
public:
    MoveRight(int index, int nextIndex1) : Command(index) {
        nextCommandIndex1 = nextIndex1;
    }

    int run(char *str, int &currPosition, int size) {
        ++currPosition;
        if (currPosition >= size) {
            currPosition = 0;
        }
        return this->nextCommandIndex1;
    }
};

class NotMark : public Command {
public:
    NotMark(int index, int nextIndex1, int nextIndex2) : Command(index) {
        nextCommandIndex1 = nextIndex1;
        nextCommandIndex2 = nextIndex2;
    }

    int run(char *str, int &currPosition, int size) {
        int nextCommand = this->nextCommandIndex1;
        if (str[currPosition] == '1') {
            nextCommand = this->nextCommandIndex2;
        }
        return nextCommand;
    }
};

class Stop : public Command {
public:
    Stop(int index) : Command(index) {}

    int run(char *str, int &currPosition, int size) { throw QuitProgram(); }
};

#endif  // TASK3_POST_MACHINE_DEYMOS01_COMMANDS_H