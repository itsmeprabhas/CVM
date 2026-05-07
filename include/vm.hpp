#ifndef VM_HPP
#define VM_HPP

#include "opcodes.hpp"  // Instruction is now defined here
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <string>

class VM {
public:
    enum class InterpretResult {
        OK,
        RUNTIME_ERROR,
        COMPILE_ERROR
    };
    
private:
    std::vector<int32_t> stack;
    static const size_t STACK_MAX = 256;
    
    std::vector<int32_t> variables;
    
    std::vector<Instruction> instructions;
    std::vector<int32_t> constants;
    
    size_t ip;
    bool running;
    
    void push(int32_t value);
    int32_t pop();
    int32_t peek(size_t offset = 0);
    
    void runtimeError(const std::string& message);
    
public:
    VM();
    
    InterpretResult loadBytecode(const std::vector<uint8_t>& bytecode);
    
    InterpretResult loadFromCompiler(const std::vector<Instruction>& instrs,
                                     const std::vector<int32_t>& consts,
                                     int32_t varCount);
    
    InterpretResult run();
    
    void reset();
    
    void printStack() const;
    
    const std::string& getLastOutput() const { return lastOutput; }
    
private:
    std::string lastOutput;
};

#endif // VM_HPP