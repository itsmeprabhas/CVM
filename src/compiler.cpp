#include "compiler.hpp"
#include <iostream>
#include <cstring>

Compiler::Compiler() : varCount(0) {}

int32_t Compiler::addConstant(int32_t value) {
    constants.push_back(value);
    return static_cast<int32_t>(constants.size() - 1);
}

int32_t Compiler::addVariable(const std::string& name) {
    if (variables.find(name) != variables.end()) {
        return variables[name];  // Already exists
    }
    int32_t index = varCount++;
    variables[name] = index;
    return index;
}

int32_t Compiler::getVariableIndex(const std::string& name) {
    auto it = variables.find(name);
    if (it == variables.end()) {
        std::cerr << "Compiler Error: Undefined variable '" << name << "'\n";
        return -1;
    }
    return it->second;
}

size_t Compiler::currentOffset() const {
    return instructions.size();
}

void Compiler::emit(OpCode op, int32_t operand) {
    instructions.emplace_back(op, operand);
}

void Compiler::patchJump(size_t offset, int32_t address) {
    instructions[offset].operand = address;
}

void Compiler::compile(Program* program) {
    program->accept(this);
    emit(OpCode::HALT);
}

void Compiler::visit(NumberLiteral* node) {
    int32_t constIndex = addConstant(node->value);
    emit(OpCode::PUSH, constIndex);
}

void Compiler::visit(BooleanLiteral* node) {
    int32_t constIndex = addConstant(node->value ? 1 : 0);
    emit(OpCode::PUSH, constIndex);
}

void Compiler::visit(Identifier* node) {
    int32_t varIndex = getVariableIndex(node->name);
    emit(OpCode::LOAD, varIndex);
}

void Compiler::visit(BinaryExpr* node) {
    node->left->accept(this);
    node->right->accept(this);
    
    if (node->op == "+")      emit(OpCode::ADD);
    else if (node->op == "-") emit(OpCode::SUB);
    else if (node->op == "*") emit(OpCode::MUL);
    else if (node->op == "/") emit(OpCode::DIV);
    else if (node->op == "==") emit(OpCode::EQ);
    else if (node->op == "<")  emit(OpCode::LT);
}

void Compiler::visit(AssignExpr* node) {
    node->value->accept(this);
    int32_t varIndex = getVariableIndex(node->name);
    emit(OpCode::STORE, varIndex);
}

void Compiler::visit(VarDecl* node) {
    int32_t varIndex = addVariable(node->name);
    node->initializer->accept(this);
    emit(OpCode::STORE, varIndex);
}

void Compiler::visit(PrintStmt* node) {
    node->expression->accept(this);
    emit(OpCode::PRINT);
}

void Compiler::visit(InputStmt* node) {
    emit(OpCode::INPUT);
    int32_t varIndex = addVariable(node->varName);
    emit(OpCode::STORE, varIndex);
}

void Compiler::visit(IfStmt* node) {
    node->condition->accept(this);
    
    // Placeholder for jump offset
    size_t jumpToElse = currentOffset();
    emit(OpCode::JUMP_IF_FALSE, 0);  // Will be patched
    
    node->thenBranch->accept(this);
    
    // If there's an else branch, we need to jump over it
    size_t jumpToEnd = 0;
    if (node->elseBranch) {
        jumpToEnd = currentOffset();
        emit(OpCode::JUMP, 0);  // Will be patched
    }
    
    // Patch the JUMP_IF_FALSE to jump here (or to else)
    patchJump(jumpToElse, static_cast<int32_t>(currentOffset()));
    
    if (node->elseBranch) {
        node->elseBranch->accept(this);
        patchJump(jumpToEnd, static_cast<int32_t>(currentOffset()));
    }
}

void Compiler::visit(WhileStmt* node) {
    // Remember the start of the loop
    size_t loopStart = currentOffset();
    
    node->condition->accept(this);
    
    // Placeholder for jump out of loop
    size_t jumpOut = currentOffset();
    emit(OpCode::JUMP_IF_FALSE, 0);  // Will be patched
    
    node->body->accept(this);
    
    // Jump back to loop start
    emit(OpCode::JUMP, static_cast<int32_t>(loopStart));
    
    // Patch the JUMP_IF_FALSE to jump here
    patchJump(jumpOut, static_cast<int32_t>(currentOffset()));
}

void Compiler::visit(Block* node) {
    for (auto& stmt : node->statements) {
        stmt->accept(this);
    }
}

void Compiler::visit(Program* node) {
    for (auto& stmt : node->statements) {
        stmt->accept(this);
    }
}

std::vector<uint8_t> Compiler::getBytecode() const {
    std::vector<uint8_t> bytecode;
    
    // Header: number of constants
    uint32_t numConstants = static_cast<uint32_t>(constants.size());
    bytecode.insert(bytecode.end(), 
                    reinterpret_cast<const uint8_t*>(&numConstants),
                    reinterpret_cast<const uint8_t*>(&numConstants) + sizeof(numConstants));
    
    // Constants
    for (int32_t constant : constants) {
        bytecode.insert(bytecode.end(),
                        reinterpret_cast<const uint8_t*>(&constant),
                        reinterpret_cast<const uint8_t*>(&constant) + sizeof(constant));
    }
    
    // Number of variables
    uint32_t numVars = static_cast<uint32_t>(varCount);
    bytecode.insert(bytecode.end(),
                    reinterpret_cast<const uint8_t*>(&numVars),
                    reinterpret_cast<const uint8_t*>(&numVars) + sizeof(numVars));
    
    // Instructions
    for (const auto& instr : instructions) {
        bytecode.push_back(static_cast<uint8_t>(instr.opcode));
        bytecode.insert(bytecode.end(),
                        reinterpret_cast<const uint8_t*>(&instr.operand),
                        reinterpret_cast<const uint8_t*>(&instr.operand) + sizeof(instr.operand));
    }
    
    return bytecode;
}

void Compiler::printBytecode() const {
    std::cout << "\n========== BYTECODE ==========\n";
    
    std::cout << "\nConstants:\n";
    for (size_t i = 0; i < constants.size(); ++i) {
        std::cout << "  [" << i << "] " << constants[i] << "\n";
    }
    
    std::cout << "\nVariables:\n";
    for (const auto& [name, index] : variables) {
        std::cout << "  [" << index << "] " << name << "\n";
    }
    
    std::cout << "\nInstructions:\n";
    for (size_t i = 0; i < instructions.size(); ++i) {
        const auto& instr = instructions[i];
        std::cout << "  " << i << ": " << opcodeToString(instr.opcode);
        if (instr.operand != 0 || instr.opcode == OpCode::PUSH || 
            instr.opcode == OpCode::LOAD || instr.opcode == OpCode::STORE ||
            instr.opcode == OpCode::JUMP || instr.opcode == OpCode::JUMP_IF_FALSE) {
            std::cout << " " << instr.operand;
        }
        std::cout << "\n";
    }
    
    std::cout << "==============================\n\n";
}