#include "ast.hpp"
#include <sstream>

// toString implementations
std::string NumberLiteral::toString() const {
    return std::to_string(value);
}

std::string BooleanLiteral::toString() const {
    return value ? "true" : "false";
}

std::string Identifier::toString() const {
    return name;
}

std::string BinaryExpr::toString() const {
    return "(" + left->toString() + " " + op + " " + right->toString() + ")";
}

std::string UnaryExpr::toString() const {
    return "(" + op + " " + operand->toString() + ")";
}

std::string AssignExpr::toString() const {
    return name + " = " + value->toString();
}

std::string VarDecl::toString() const {
    return "let " + name + " = " + initializer->toString();
}

std::string PrintStmt::toString() const {
    return "print " + expression->toString();
}

std::string InputStmt::toString() const {
    return "input " + varName;
}

std::string IfStmt::toString() const {
    std::string result = "if (" + condition->toString() + ") " + thenBranch->toString();
    if (elseBranch) {
        result += " else " + elseBranch->toString();
    }
    return result;
}

std::string WhileStmt::toString() const {
    return "while (" + condition->toString() + ") " + body->toString();
}

std::string Block::toString() const {
    std::string result = "{\n";
    for (const auto& stmt : statements) {
        result += "  " + stmt->toString() + ";\n";
    }
    result += "}";
    return result;
}

std::string Program::toString() const {
    std::string result;
    for (const auto& stmt : statements) {
        result += stmt->toString() + ";\n";
    }
    return result;
}

// ASTPrinter implementation
void ASTPrinter::printIndent() {
    for (int i = 0; i < indent; ++i) {
        output += "  ";
    }
}

void ASTPrinter::visit(NumberLiteral* node) {
    printIndent();
    output += "NumberLiteral: " + std::to_string(node->value) + "\n";
}

void ASTPrinter::visit(BooleanLiteral* node) {
    printIndent();
    output += "BooleanLiteral: " + std::string(node->value ? "true" : "false") + "\n";
}

void ASTPrinter::visit(Identifier* node) {
    printIndent();
    output += "Identifier: " + node->name + "\n";
}

void ASTPrinter::visit(BinaryExpr* node) {
    printIndent();
    output += "BinaryExpr: '" + node->op + "'\n";
    indent++;
    output += "[Left]\n";
    indent++;
    node->left->accept(this);
    indent--;
    output += "[Right]\n";
    indent++;
    node->right->accept(this);
    indent--;
    indent--;
}

void ASTPrinter::visit(UnaryExpr* node) {
    printIndent();
    output += "UnaryExpr: '" + node->op + "'\n";
    indent++;
    node->operand->accept(this);
    indent--;
}

void ASTPrinter::visit(AssignExpr* node) {
    printIndent();
    output += "AssignExpr: '" + node->name + "'\n";
    indent++;
    node->value->accept(this);
    indent--;
}

void ASTPrinter::visit(VarDecl* node) {
    printIndent();
    output += "VarDecl: '" + node->name + "'\n";
    indent++;
    node->initializer->accept(this);
    indent--;
}

void ASTPrinter::visit(PrintStmt* node) {
    printIndent();
    output += "PrintStmt\n";
    indent++;
    node->expression->accept(this);
    indent--;
}

void ASTPrinter::visit(InputStmt* node) {
    printIndent();
    output += "InputStmt: '" + node->varName + "'\n";
}

void ASTPrinter::visit(IfStmt* node) {
    printIndent();
    output += "IfStmt\n";
    indent++;
    output += "[Condition]\n";
    indent++;
    node->condition->accept(this);
    indent--;
    output += "[Then]\n";
    indent++;
    node->thenBranch->accept(this);
    indent--;
    if (node->elseBranch) {
        output += "[Else]\n";
        indent++;
        node->elseBranch->accept(this);
        indent--;
    }
    indent--;
}

void ASTPrinter::visit(WhileStmt* node) {
    printIndent();
    output += "WhileStmt\n";
    indent++;
    output += "[Condition]\n";
    indent++;
    node->condition->accept(this);
    indent--;
    output += "[Body]\n";
    indent++;
    node->body->accept(this);
    indent--;
    indent--;
}

void ASTPrinter::visit(Block* node) {
    printIndent();
    output += "Block (" + std::to_string(node->statements.size()) + " statements)\n";
    indent++;
    for (auto& stmt : node->statements) {
        stmt->accept(this);
    }
    indent--;
}

void ASTPrinter::visit(Program* node) {
    output += "Program (" + std::to_string(node->statements.size()) + " statements)\n";
    indent++;
    for (auto& stmt : node->statements) {
        stmt->accept(this);
    }
    indent--;
}