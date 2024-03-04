#ifndef TASK3_POST_MACHINE_DEYMOS01_ERRORS_H
#define TASK3_POST_MACHINE_DEYMOS01_ERRORS_H

class Error {};

class ErrorSyntaxProgram : public Error {};

class ErrorOutTimeLimit : public Error {};

class QuitProgram : public Error {};

#endif  // TASK3_POST_MACHINE_DEYMOS01_ERRORS_H