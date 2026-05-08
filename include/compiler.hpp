#ifndef COMPILER_HPP
#define COMPILER_HPP

#include "ast.hpp"
#include "opcodes.hpp"  // Instruction is now defined here
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <string>

class Compiler : public ASTVisitor {
private:
    std::vector<Instruction> instructions;
    std::vector<int32_t> constants;
    std::unordered_map<std::string, int32_t> variables;
    int32_t varCount;
    
    // Helper methods
    int32_t addConstant(int32_t value);
    int32_t addVariable(const std::string& name);
    int32_t getVariableIndex(const std::string& name);
    size_t currentOffset() const;
    void emit(OpCode op, int32_t operand = 0);
    void patchJump(size_t offset, int32_t address);
    
    // Visitor implementations
    void visit(NumberLiteral* node) override;
    void visit(BooleanLiteral* node) override;
    void visit(Identifier* node) override;
    void visit(BinaryExpr* node) override;
    void visit(UnaryExpr* node) override;
    void visit(AssignExpr* node) override;
    void visit(VarDecl* node) override;
    void visit(PrintStmt* node) override;
    void visit(InputStmt* node) override;
    void visit(IfStmt* node) override;
    void visit(WhileStmt* node) override;
    void visit(Block* node) override;
    void visit(Program* node) override;
    
public:
    Compiler();
    
    void compile(Program* program);
    
    std::vector<uint8_t> getBytecode() const;
    
    const std::vector<Instruction>& getInstructions() const { return instructions; }
    const std::vector<int32_t>& getConstants() const { return constants; }
    const std::unordered_map<std::string, int32_t>& getVariables() const { return variables; }
    
    void printBytecode() const;
};

#endif // COMPILER_HPP