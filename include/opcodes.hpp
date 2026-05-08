#ifndef OPCODES_HPP
#define OPCODES_HPP

#include <cstdint>
#include <string>

// Opcodes for our stack-based VM
enum class OpCode : uint8_t {
    // Stack Operations
    PUSH,           
    POP,            
    DUP,
    
    // Arithmetic Operations
    ADD,            
    SUB,            
    MUL,            
    DIV,            
    MOD,
    
    // Comparison Operations
    EQ,             
    NEQ,
    LT,             
    LE,
    GT,
    GE,
    
    // Bitwise Operations
    BIT_AND,
    BIT_OR,
    BIT_XOR,
    SHL,
    SHR,
    
    // Logical/Unary Operations
    AND,
    OR,
    NOT,
    
    // Variable Operations
    LOAD,           
    STORE,          
    
    // I/O Operations
    PRINT,          
    INPUT,          
    
    // Control Flow
    JUMP,           
    JUMP_IF_FALSE,  
    JUMP_IF_TRUE,   
    
    // Program Control
    HALT,           
    NOP             
};

// Convert opcode to string for debugging
std::string opcodeToString(OpCode op);

// ADD THIS STRUCT HERE - Shared between Compiler and VM
struct Instruction {
    OpCode opcode;
    int32_t operand;
    
    Instruction(OpCode op, int32_t oper = 0) : opcode(op), operand(oper) {}
};

#endif // OPCODES_HPP