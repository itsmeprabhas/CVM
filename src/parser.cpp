#include "parser.hpp"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) 
    : tokens(tokens), current(0) {}

Token& Parser::peek() {
    return tokens[current];
}

Token& Parser::previous() {
    return tokens[current - 1];
}

Token& Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::EOF_TOKEN;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

Token& Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    
    Token& tok = peek();
    throw ParseError(message, tok.line, tok.column);
}

std::unique_ptr<Program> Parser::parse() {
    return parseProgram();
}

std::unique_ptr<Program> Parser::parseProgram() {
    std::vector<std::unique_ptr<ASTNode>> statements;
    
    while (!isAtEnd()) {
        statements.push_back(parseDeclaration());
    }
    
    return std::make_unique<Program>(std::move(statements));
}

std::unique_ptr<ASTNode> Parser::parseDeclaration() {
    if (match(TokenType::LET)) {
        // Variable declaration: let <name> = <expr>;
        Token name = consume(TokenType::IDENTIFIER, "Expected variable name after 'let'");
        consume(TokenType::ASSIGN, "Expected '=' after variable name");
        std::unique_ptr<ASTNode> initializer = parseExpression();
        consume(TokenType::SEMICOLON, "Expected ';' after variable declaration");
        return std::make_unique<VarDecl>(name.lexeme, std::move(initializer));
    }
    
    return parseStatement();
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (match(TokenType::PRINT)) {
        return parsePrintStatement();
    }
    if (match(TokenType::INPUT)) {
        return parseInputStatement();
    }
    if (match(TokenType::IF)) {
        return parseIfStatement();
    }
    if (match(TokenType::WHILE)) {
        return parseWhileStatement();
    }
    if (match(TokenType::LBRACE)) {
        auto block = parseBlock();
        return std::move(block);
    }
    
    return parseExpressionStatement();
}

std::unique_ptr<Block> Parser::parseBlock() {
    std::vector<std::unique_ptr<ASTNode>> statements;
    
    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        statements.push_back(parseDeclaration());
    }
    
    consume(TokenType::RBRACE, "Expected '}' after block");
    return std::make_unique<Block>(std::move(statements));
}

std::unique_ptr<ASTNode> Parser::parseIfStatement() {
    consume(TokenType::LPAREN, "Expected '(' after 'if'");
    std::unique_ptr<ASTNode> condition = parseExpression();
    consume(TokenType::RPAREN, "Expected ')' after if condition");
    
    consume(TokenType::LBRACE, "Expected '{' after if condition");
    std::unique_ptr<Block> thenBranch = parseBlock();
    
    std::unique_ptr<Block> elseBranch = nullptr;
    if (match(TokenType::ELSE)) {
        consume(TokenType::LBRACE, "Expected '{' after 'else'");
        elseBranch = parseBlock();
    }
    
    return std::make_unique<IfStmt>(std::move(condition), 
                                    std::move(thenBranch),
                                    std::move(elseBranch));
}

std::unique_ptr<ASTNode> Parser::parseWhileStatement() {
    consume(TokenType::LPAREN, "Expected '(' after 'while'");
    std::unique_ptr<ASTNode> condition = parseExpression();
    consume(TokenType::RPAREN, "Expected ')' after while condition");
    
    consume(TokenType::LBRACE, "Expected '{' after while condition");
    std::unique_ptr<Block> body = parseBlock();
    
    return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
}

std::unique_ptr<ASTNode> Parser::parsePrintStatement() {
    std::unique_ptr<ASTNode> expr = parseExpression();
    consume(TokenType::SEMICOLON, "Expected ';' after print statement");
    return std::make_unique<PrintStmt>(std::move(expr));
}

std::unique_ptr<ASTNode> Parser::parseInputStatement() {
    Token name = consume(TokenType::IDENTIFIER, "Expected variable name after 'input'");
    consume(TokenType::SEMICOLON, "Expected ';' after input statement");
    return std::make_unique<InputStmt>(name.lexeme);
}

std::unique_ptr<ASTNode> Parser::parseExpressionStatement() {
    std::unique_ptr<ASTNode> expr = parseExpression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression");
    return std::move(expr);
}

// Expression parsing with operator precedence
std::unique_ptr<ASTNode> Parser::parseExpression() {
    return parseAssignment();
}

std::unique_ptr<ASTNode> Parser::parseAssignment() {
    std::unique_ptr<ASTNode> expr = parseEquality();
    
    if (match(TokenType::ASSIGN)) {
        Token equals = previous();
        std::unique_ptr<ASTNode> value = parseAssignment();
        
        // Check if left side is an identifier
        if (auto* ident = dynamic_cast<Identifier*>(expr.get())) {
            std::string name = ident->name;
            return std::make_unique<AssignExpr>(name, std::move(value));
        }
        
        throw ParseError("Invalid assignment target", equals.line, equals.column);
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseEquality() {
    std::unique_ptr<ASTNode> left = parseComparison();
    
    while (match(TokenType::EQ)) {
        Token op = previous();
        std::unique_ptr<ASTNode> right = parseComparison();
        left = std::make_unique<BinaryExpr>(std::move(left), op.lexeme, std::move(right));
    }
    
    return left;
}

std::unique_ptr<ASTNode> Parser::parseComparison() {
    std::unique_ptr<ASTNode> left = parseTerm();
    
    while (match(TokenType::LT)) {
        Token op = previous();
        std::unique_ptr<ASTNode> right = parseTerm();
        left = std::make_unique<BinaryExpr>(std::move(left), op.lexeme, std::move(right));
    }
    
    return left;
}

std::unique_ptr<ASTNode> Parser::parseTerm() {
    std::unique_ptr<ASTNode> left = parseFactor();
    
    while (check(TokenType::PLUS) || check(TokenType::MINUS)) {
        Token op = advance();
        std::unique_ptr<ASTNode> right = parseFactor();
        left = std::make_unique<BinaryExpr>(std::move(left), op.lexeme, std::move(right));
    }
    
    return left;
}

std::unique_ptr<ASTNode> Parser::parseFactor() {
    std::unique_ptr<ASTNode> left = parseUnary();
    
    while (check(TokenType::STAR) || check(TokenType::SLASH)) {
        Token op = advance();
        std::unique_ptr<ASTNode> right = parseUnary();
        left = std::make_unique<BinaryExpr>(std::move(left), op.lexeme, std::move(right));
    }
    
    return left;
}

std::unique_ptr<ASTNode> Parser::parseUnary() {
    // For now, no unary operators - can add negation later
    return parsePrimary();
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    if (match(TokenType::NUMBER)) {
        return std::make_unique<NumberLiteral>(std::stoi(previous().lexeme));
    }
    
    if (match(TokenType::BOOLEAN)) {
        bool val = (previous().lexeme == "true");
        return std::make_unique<BooleanLiteral>(val);
    }
    
    if (match(TokenType::IDENTIFIER)) {
        return std::make_unique<Identifier>(previous().lexeme);
    }
    
    if (match(TokenType::LPAREN)) {
        std::unique_ptr<ASTNode> expr = parseExpression();
        consume(TokenType::RPAREN, "Expected ')' after expression");
        return expr;
    }
    
    Token& tok = peek();
    throw ParseError("Expected expression", tok.line, tok.column);
}