#include "opcodes.hpp"

std::string opcodeToString(OpCode op) {
    switch (op) {
        case OpCode::PUSH:          return "PUSH";
        case OpCode::POP:           return "POP";
        case OpCode::DUP:           return "DUP";
        case OpCode::ADD:           return "ADD";
        case OpCode::SUB:           return "SUB";
        case OpCode::MUL:           return "MUL";
        case OpCode::DIV:           return "DIV";
        case OpCode::MOD:           return "MOD";
        case OpCode::EQ:            return "EQ";
        case OpCode::NEQ:           return "NEQ";
        case OpCode::LT:            return "LT";
        case OpCode::LE:            return "LE";
        case OpCode::GT:            return "GT";
        case OpCode::GE:            return "GE";
        case OpCode::BIT_AND:       return "BIT_AND";
        case OpCode::BIT_OR:        return "BIT_OR";
        case OpCode::BIT_XOR:       return "BIT_XOR";
        case OpCode::SHL:           return "SHL";
        case OpCode::SHR:           return "SHR";
        case OpCode::AND:           return "AND";
        case OpCode::OR:            return "OR";
        case OpCode::NOT:           return "NOT";
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