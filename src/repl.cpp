#include "repl.hpp"
#include "ast.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

REPL::REPL() : debugMode(false) {}

void REPL::printBanner() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║           CVM++ Virtual Machine       ║\n";
    std::cout << "║    Stack-Based VM & Custom Compiler   ║\n";
    std::cout << "║          Version 1.0.0                ║\n";
    std::cout << "╠══════════════════════════════════════╣\n";
    std::cout << "║  Commands:                            ║\n";
    std::cout << "║    .debug   - Toggle debug mode       ║\n";
    std::cout << "║    .quit    - Exit REPL               ║\n";
    std::cout << "║    .help    - Show this help          ║\n";
    std::cout << "╚══════════════════════════════════════╝\n\n";
}

void REPL::run(int argc, char* argv[]) {
    if (argc > 1) {
        // Check for debug flag
        std::string filename = argv[1];
        if (argc > 2 && std::string(argv[2]) == "--debug") {
            debugMode = true;
        }
        runFile(filename);
    } else {
        runInteractive();
    }
}

void REPL::runFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'\n";
        return;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    
    std::cout << "Running file: " << filename << "\n";
    execute(source);
}

void REPL::runInteractive() {
    printBanner();
    
    std::string line;
    std::string source;
    
    while (true) {
        std::cout << "cvm++> ";
        std::getline(std::cin, line);
        
        if (line.empty()) continue;
        
        // Handle commands
        if (line[0] == '.') {
            if (line == ".quit" || line == ".exit") {
                std::cout << "Goodbye!\n";
                break;
            } else if (line == ".debug") {
                debugMode = !debugMode;
                std::cout << "Debug mode: " << (debugMode ? "ON" : "OFF") << "\n";
                continue;
            } else if (line == ".help") {
                printBanner();
                continue;
            } else {
                std::cout << "Unknown command: " << line << "\n";
                continue;
            }
        }
        
        execute(line);
    }
}

void REPL::execute(const std::string& source) {
    try {
        // Step 1: Lexing
        if (debugMode) {
            std::cout << "\n===== LEXING =====\n";
        }
        
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();
        
        if (debugMode) {
            for (const auto& token : tokens) {
                std::cout << token.toString() << "\n";
            }
        }
        
        // Step 2: Parsing
        if (debugMode) {
            std::cout << "\n===== PARSING =====\n";
        }
        
        Parser parser(tokens);
        std::unique_ptr<Program> program = parser.parse();
        
        if (debugMode) {
            ASTPrinter printer;
            program->accept(&printer);
            std::cout << printer.getOutput();
        }
        
        // Step 3: Compilation
        if (debugMode) {
            std::cout << "\n===== COMPILATION =====\n";
        }
        
        Compiler compiler;
        compiler.compile(program.get());
        
        if (debugMode) {
            compiler.printBytecode();
        }
        
        // Step 4: Execution
        if (debugMode) {
            std::cout << "\n===== EXECUTION =====\n";
        }
        
        VM vm;
        vm.loadFromCompiler(compiler.getInstructions(), 
                           compiler.getConstants(),
                           compiler.getVariables().size());
        
        VM::InterpretResult result = vm.run();
        
        if (result == VM::InterpretResult::OK) {
            if (debugMode) {
                std::cout << "Execution completed successfully.\n";
            }
        } else {
            std::cout << "Execution terminated with errors.\n";
        }
        
        if (debugMode) {
            vm.printStack();
        }
        
        std::cout << "\n";
        
    } catch (const LexError& e) {
        std::cerr << "[Lex Error] Line " << e.line << ": " << e.what() << "\n";
    } catch (const ParseError& e) {
        std::cerr << "[Parse Error] Line " << e.line << ": " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
    }
}