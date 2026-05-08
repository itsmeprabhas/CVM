#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <string>
#include <vector>
#include <variant>

// Forward declarations
class ASTNode;
class NumberLiteral;
class BooleanLiteral;
class Identifier;
class BinaryExpr;
class UnaryExpr;
class AssignExpr;
class VarDecl;
class PrintStmt;
class InputStmt;
class IfStmt;
class WhileStmt;
class Block;
class Program;

// Visitor pattern for AST traversal
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    virtual void visit(NumberLiteral* node) = 0;
    virtual void visit(BooleanLiteral* node) = 0;
    virtual void visit(Identifier* node) = 0;
    virtual void visit(BinaryExpr* node) = 0;
    virtual void visit(UnaryExpr* node) = 0;
    virtual void visit(AssignExpr* node) = 0;
    virtual void visit(VarDecl* node) = 0;
    virtual void visit(PrintStmt* node) = 0;
    virtual void visit(InputStmt* node) = 0;
    virtual void visit(IfStmt* node) = 0;
    virtual void visit(WhileStmt* node) = 0;
    virtual void visit(Block* node) = 0;
    virtual void visit(Program* node) = 0;
};

// Base AST Node
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor* visitor) = 0;
    virtual std::string toString() const = 0;
};

// Expressions
class NumberLiteral : public ASTNode {
public:
    int value;
    explicit NumberLiteral(int val) : value(val) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    std::string toString() const override;
};

class BooleanLiteral : public ASTNode {
public:
    bool value;
    explicit BooleanLiteral(bool val) : value(val) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    std::string toString() const override;
};

class Identifier : public ASTNode {
public:
    std::string name;
    explicit Identifier(const std::string& n) : name(n) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    std::string toString() const override;
};

class BinaryExpr : public ASTNode {
public:
    std::unique_ptr<ASTNode> left;
    std::string op;  // "+", "-", "*", "/", "==", "<"
    std::unique_ptr<ASTNode> right;
    
    BinaryExpr(std::unique_ptr<ASTNode> l, const std::string& o, 
               std::unique_ptr<ASTNode> r)
        : left(std::move(l)), op(o), right(std::move(r)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    std::string toString() const override;
};

class UnaryExpr : public ASTNode {
public:
    std::string op;
    std::unique_ptr<ASTNode> operand;
    
    UnaryExpr(const std::string& o, std::unique_ptr<ASTNode> value)
        : op(o), operand(std::move(value)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    std::string toString() const override;
};

class AssignExpr : public ASTNode {
public:
    std::string name;
    std::unique_ptr<ASTNode> value;
    
    AssignExpr(const std::string& n, std::unique_ptr<ASTNode> v)
        : name(n), value(std::move(v)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    std::string toString() const override;
};

// Statements
class VarDecl : public ASTNode {
public:
    std::string name;
    std::unique_ptr<ASTNode> initializer;
    
    VarDecl(const std::string& n, std::unique_ptr<ASTNode> init)
        : name(n), initializer(std::move(init)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    std::string toString() const override;
};

class PrintStmt : public ASTNode {
public:
    std::unique_ptr<ASTNode> expression;
    
    explicit PrintStmt(std::unique_ptr<ASTNode> expr)
        : expression(std::move(expr)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    std::string toString() const override;
};

class InputStmt : public ASTNode {
public:
    std::string varName;
    
    explicit InputStmt(const std::string& name) : varName(name) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    std::string toString() const override;
};

class IfStmt : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<Block> thenBranch;
    std::unique_ptr<Block> elseBranch;
    
    IfStmt(std::unique_ptr<ASTNode> cond, std::unique_ptr<Block> thenB,
           std::unique_ptr<Block> elseB = nullptr)
        : condition(std::move(cond)), thenBranch(std::move(thenB)),
          elseBranch(std::move(elseB)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    std::string toString() const override;
};

class WhileStmt : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<Block> body;
    
    WhileStmt(std::unique_ptr<ASTNode> cond, std::unique_ptr<Block> b)
        : condition(std::move(cond)), body(std::move(b)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    std::string toString() const override;
};

class Block : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;
    
    explicit Block(std::vector<std::unique_ptr<ASTNode>> stmts)
        : statements(std::move(stmts)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    std::string toString() const override;
};

class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;
    
    explicit Program(std::vector<std::unique_ptr<ASTNode>> stmts)
        : statements(std::move(stmts)) {}
    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    std::string toString() const override;
};

// AST Printer for debugging
class ASTPrinter : public ASTVisitor {
private:
    int indent;
    std::string output;
    
    void printIndent();
public:
    ASTPrinter() : indent(0) {}
    
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
    
    std::string getOutput() const { return output; }
};

#endif // AST_HPP