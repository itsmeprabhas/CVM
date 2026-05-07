#include "repl.hpp"

int main(int argc, char* argv[]) {
    REPL repl;
    
    // Check for global debug flag
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--debug") {
            repl.setDebugMode(true);
        }
    }
    
    repl.run(argc, argv);
    return 0;
}