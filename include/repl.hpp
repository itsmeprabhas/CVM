#ifndef REPL_HPP
#define REPL_HPP

#include "lexer.hpp"
#include "parser.hpp"
#include "compiler.hpp"
#include "vm.hpp"

class REPL {
private:
    bool debugMode;
    
    void printBanner();
    void runFile(const std::string& filename);
    void runInteractive();
    void execute(const std::string& source);
    
public:
    REPL();
    
    void run(int argc, char* argv[]);
    
    void setDebugMode(bool enabled) { debugMode = enabled; }
};

#endif // REPL_HPP