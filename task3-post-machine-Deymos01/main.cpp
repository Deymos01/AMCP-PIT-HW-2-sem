#include "postmachine.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        return -1;
    }

    std::ifstream inFileProgram(argv[1]);
    std::ifstream inFileConfig(argv[2]);

    int sizeOfTape;
    int maxIter;
    inFileConfig >> sizeOfTape;
    inFileConfig >> maxIter;
    inFileConfig.ignore();  // Ignore space after maxIter

    // Record data from inputFile
    std::string init(sizeOfTape + 2, '\0');
    char currSymb;
    for (int i = 0; (currSymb = inFileConfig.get()) != EOF && i < sizeOfTape; ++i){
        init[i] = currSymb;
    }
    inFileConfig.close();

    std::ofstream outFile(argv[3]);
    BinTree *program = new BinTree;
    try {
        // load program from input file
        loadProgram(inFileProgram, program);
        // Create PostMachine and load program
        PostMachine pm(sizeOfTape, program);
        // Calc result
        std::string result = pm.calc(init, maxIter);
        outFile << result;
    } catch (ErrorSyntaxProgram error) {  // Error program loading
        outFile << "Error";
    } catch (ErrorOutTimeLimit error) {  // Exceeded time limit
        outFile << "Not applicable";
    } catch (...) {}
    delete program;
    inFileProgram.close();
    outFile.close();
    return 0;
}