#include "vm.hpp"
#include <iostream>
#include <cstring>
#include <sstream>

VM::VM() : ip(0), running(false) {
    stack.reserve(STACK_MAX);
}

void VM::push(int32_t value) {
    if (stack.size() >= STACK_MAX) {
        runtimeError("Stack overflow");
        return;
    }
    stack.push_back(value);
}

int32_t VM::pop() {
    if (stack.empty()) {
        runtimeError("Stack underflow");
        return 0;
    }
    int32_t value = stack.back();
    stack.pop_back();
    return value;
}

int32_t VM::peek(size_t offset) {
    if (stack.size() <= offset) {
        runtimeError("Stack underflow on peek");
        return 0;
    }
    return stack[stack.size() - 1 - offset];
}

void VM::runtimeError(const std::string& message) {
    std::cerr << "[Runtime Error] at instruction " << ip << ": " << message << "\n";
    running = false;
}

VM::InterpretResult VM::loadBytecode(const std::vector<uint8_t>& bytecode) {
    if (bytecode.size() < sizeof(uint32_t)) {
        return InterpretResult::COMPILE_ERROR;
    }
    
    size_t pos = 0;
    
    // Read number of constants
    uint32_t numConstants;
    std::memcpy(&numConstants, &bytecode[pos], sizeof(numConstants));
    pos += sizeof(numConstants);
    
    // Read constants
    constants.clear();
    for (uint32_t i = 0; i < numConstants; ++i) {
        int32_t constant;
        std::memcpy(&constant, &bytecode[pos], sizeof(constant));
        pos += sizeof(constant);
        constants.push_back(constant);
    }
    
    // Read number of variables
    uint32_t numVars;
    std::memcpy(&numVars, &bytecode[pos], sizeof(numVars));
    pos += sizeof(numVars);
    
    // Initialize variables
    variables.assign(numVars, 0);
    
    // Read instructions
    instructions.clear();
    while (pos < bytecode.size()) {
        OpCode opcode = static_cast<OpCode>(bytecode[pos++]);
        int32_t operand = 0;
        if (pos + sizeof(operand) <= bytecode.size()) {
            std::memcpy(&operand, &bytecode[pos], sizeof(operand));
            pos += sizeof(operand);
        }
        instructions.emplace_back(opcode, operand);
    }
    
    ip = 0;
    return InterpretResult::OK;
}

VM::InterpretResult VM::loadFromCompiler(const std::vector<Instruction>& instrs,
                                         const std::vector<int32_t>& consts,
                                         int32_t varCount) {
    instructions = instrs;
    constants = consts;
    variables.assign(varCount, 0);
    ip = 0;
    return InterpretResult::OK;
}

void VM::reset() {
    stack.clear();
    variables.clear();
    instructions.clear();
    constants.clear();
    ip = 0;
    running = false;
    lastOutput.clear();
}

void VM::printStack() const {
    std::cout << "Stack: [";
    for (size_t i = 0; i < stack.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << stack[i];
    }
    std::cout << "]\n";
}

VM::InterpretResult VM::run() {
    running = true;
    
    while (running && ip < instructions.size()) {
        const Instruction& instr = instructions[ip];
        
        switch (instr.opcode) {
            case OpCode::PUSH: {
                int32_t value = constants[instr.operand];
                push(value);
                ip++;
                break;
            }
            
            case OpCode::POP: {
                pop();
                ip++;
                break;
            }

            case OpCode::DUP: {
                int32_t value = peek();
                push(value);
                ip++;
                break;
            }
            
            case OpCode::ADD: {
                int32_t b = pop();
                int32_t a = pop();
                push(a + b);
                ip++;
                break;
            }
            
            case OpCode::SUB: {
                int32_t b = pop();
                int32_t a = pop();
                push(a - b);
                ip++;
                break;
            }
            
            case OpCode::MUL: {
                int32_t b = pop();
                int32_t a = pop();
                push(a * b);
                ip++;
                break;
            }
            
            case OpCode::DIV: {
                int32_t b = pop();
                int32_t a = pop();
                if (b == 0) {
                    runtimeError("Division by zero");
                    return InterpretResult::RUNTIME_ERROR;
                }
                push(a / b);
                ip++;
                break;
            }
            
            case OpCode::MOD: {
                int32_t b = pop();
                int32_t a = pop();
                if (b == 0) {
                    runtimeError("Modulo by zero");
                    return InterpretResult::RUNTIME_ERROR;
                }
                push(a % b);
                ip++;
                break;

            }
            
            case OpCode::EQ: {
                int32_t b = pop();
                int32_t a = pop();
                push(a == b ? 1 : 0);
                ip++;
                break;
            }

            case OpCode::NEQ: {
                int32_t b = pop();
                int32_t a = pop();
                push(a != b ? 1 : 0);
                ip++;
                break;
            }
            
            case OpCode::LT: {
                int32_t b = pop();
                int32_t a = pop();
                push(a < b ? 1 : 0);
                ip++;
                break;
            }

            case OpCode::LE: {
                int32_t b = pop();
                int32_t a = pop();
                push(a <= b ? 1 : 0);
                ip++;
                break;
            }

            case OpCode::GT: {
                int32_t b = pop();
                int32_t a = pop();
                push(a > b ? 1 : 0);
                ip++;
                break;
            }

            case OpCode::GE: {
                int32_t b = pop();
                int32_t a = pop();
                push(a >= b ? 1 : 0);
                ip++;
                break;
            }

            case OpCode::BIT_AND: {
                int32_t b = pop();
                int32_t a = pop();
                push(a & b);
                ip++;
                break;
            }

            case OpCode::BIT_OR: {
                int32_t b = pop();
                int32_t a = pop();
                push(a | b);
                ip++;
                break;
            }

            case OpCode::BIT_XOR: {
                int32_t b = pop();
                int32_t a = pop();
                push(a ^ b);
                ip++;
                break;
            }

            case OpCode::SHL: {
                int32_t b = pop();
                int32_t a = pop();
                push(a << b);
                ip++;
                break;
            }

            case OpCode::SHR: {
                int32_t b = pop();
                int32_t a = pop();
                push(a >> b);
                ip++;
                break;
            }

            case OpCode::AND: {
                int32_t b = pop();
                int32_t a = pop();
                push((a != 0 && b != 0) ? 1 : 0);
                ip++;
                break;
            }

            case OpCode::OR: {
                int32_t b = pop();
                int32_t a = pop();
                push((a != 0 || b != 0) ? 1 : 0);
                ip++;
                break;
            }

            case OpCode::NOT: {
                int32_t a = pop();
                push(a == 0 ? 1 : 0);
                ip++;
                break;
            }
            
            case OpCode::LOAD: {
                if (instr.operand < 0 || instr.operand >= (int32_t)variables.size()) {
                    runtimeError("Invalid variable index");
                    return InterpretResult::RUNTIME_ERROR;
                }
                push(variables[instr.operand]);
                ip++;
                break;
            }
            
            case OpCode::STORE: {
                if (instr.operand < 0 || instr.operand >= (int32_t)variables.size()) {
                    runtimeError("Invalid variable index");
                    return InterpretResult::RUNTIME_ERROR;
                }
                variables[instr.operand] = pop();
                ip++;
                break;
            }
            
            case OpCode::PRINT: {
                int32_t value = pop();
                std::cout << value << "\n";
                lastOutput += std::to_string(value) + "\n";
                ip++;
                break;
            }
            
            case OpCode::INPUT: {
                int32_t value;
                std::cout << "> ";
                std::cin >> value;
                push(value);
                ip++;
                break;
            }
            
            case OpCode::JUMP: {
                ip = static_cast<size_t>(instr.operand);
                break;
            }
            
            case OpCode::JUMP_IF_FALSE: {
                int32_t condition = pop();
                if (condition == 0) {
                    ip = static_cast<size_t>(instr.operand);
                } else {
                    ip++;
                }
                break;
            }
            
            case OpCode::JUMP_IF_TRUE: {
                int32_t condition = pop();
                if (condition != 0) {
                    ip = static_cast<size_t>(instr.operand);
                } else {
                    ip++;
                }
                break;
            }
            
            case OpCode::HALT: {
                running = false;
                break;
            }
            
            case OpCode::NOP: {
                ip++;
                break;
            }
            
            default: {
                runtimeError("Unknown opcode");
                return InterpretResult::RUNTIME_ERROR;
            }
        }
    }
    
    return running ? InterpretResult::RUNTIME_ERROR : InterpretResult::OK;
}