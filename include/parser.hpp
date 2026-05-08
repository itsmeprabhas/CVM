#ifndef PARSER_HPP
#define PARSER_HPP

#include "token.hpp"
#include "ast.hpp"
#include <vector>
#include <memory>

class Parser {
private:
    std::vector<Token> tokens;
    size_t current;
    
    // Token navigation
    Token& peek();
    Token& previous();
    Token& advance();
    bool isAtEnd();
    bool check(TokenType type);
    bool match(TokenType type);
    Token& consume(TokenType type, const std::string& message);
    
    // Parsing rules (Grammar)
    std::unique_ptr<Program> parseProgram();
    std::unique_ptr<ASTNode> parseDeclaration();
    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<Block> parseBlock();
    std::unique_ptr<ASTNode> parseIfStatement();
    std::unique_ptr<ASTNode> parseWhileStatement();
    std::unique_ptr<ASTNode> parseForStatement();
    std::unique_ptr<ASTNode> parseCaseStatement();
    std::unique_ptr<ASTNode> parseBreakStatement();
    std::unique_ptr<ASTNode> parseContinueStatement();
    std::unique_ptr<ASTNode> parsePrintStatement();
    std::unique_ptr<ASTNode> parseInputStatement();
    std::unique_ptr<ASTNode> parseExpressionStatement();
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseAssignment();
    std::unique_ptr<ASTNode> parseLogicalOr();
    std::unique_ptr<ASTNode> parseLogicalAnd();
    std::unique_ptr<ASTNode> parseBitwiseOr();
    std::unique_ptr<ASTNode> parseBitwiseXor();
    std::unique_ptr<ASTNode> parseBitwiseAnd();
    std::unique_ptr<ASTNode> parseEquality();
    std::unique_ptr<ASTNode> parseComparison();
    std::unique_ptr<ASTNode> parseShift();
    std::unique_ptr<ASTNode> parseTerm();
    std::unique_ptr<ASTNode> parseFactor();
    std::unique_ptr<ASTNode> parseUnary();
    std::unique_ptr<ASTNode> parsePostfix();
    std::unique_ptr<ASTNode> parsePrimary();
    
public:
    Parser(const std::vector<Token>& tokens);
    
    std::unique_ptr<Program> parse();
};

class ParseError : public std::runtime_error {
public:
    int line;
    int column;
    ParseError(const std::string& msg, int ln, int col)
        : std::runtime_error(msg), line(ln), column(col) {}
};

#endif // PARSER_HPP