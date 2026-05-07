#include "opcodes.hpp"

std::string opcodeToString(OpCode op) {
    switch (op) {
        case OpCode::PUSH:          return "PUSH";
        case OpCode::POP:           return "POP";
        case OpCode::ADD:           return "ADD";
        case OpCode::SUB:           return "SUB";
        case OpCode::MUL:           return "MUL";
        case OpCode::DIV:           return "DIV";
        case OpCode::EQ:            return "EQ";
        case OpCode::LT:            return "LT";
        case OpCode::LOAD:          return "LOAD";
        case OpCode::STORE:         return "STORE";
        case OpCode::PRINT:         return "PRINT";
        case OpCode::INPUT:         return "INPUT";
        case OpCode::JUMP:          return "JUMP";
        case OpCode::JUMP_IF_FALSE: return "JUMP_IF_FALSE";
        case OpCode::JUMP_IF_TRUE:  return "JUMP_IF_TRUE";
        case OpCode::HALT:          return "HALT";
        case OpCode::NOP:           return "NOP";
        default:                    return "UNKNOWN";
    }
}