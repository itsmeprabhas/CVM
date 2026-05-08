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
    if (match(TokenType::FOR)) {
        return parseForStatement();
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

std::unique_ptr<ASTNode> Parser::parseForStatement() {
    consume(TokenType::LPAREN, "Expected '(' after 'for'");

    std::unique_ptr<ASTNode> initializer = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        if (match(TokenType::LET)) {
            Token name = consume(TokenType::IDENTIFIER, "Expected variable name after 'let'");
            consume(TokenType::ASSIGN, "Expected '=' after variable name");
            std::unique_ptr<ASTNode> initValue = parseExpression();
            initializer = std::make_unique<VarDecl>(name.lexeme, std::move(initValue));
        } else {
            initializer = parseExpression();
        }
    }
    consume(TokenType::SEMICOLON, "Expected ';' after for initializer");

    std::unique_ptr<ASTNode> condition;
    if (!check(TokenType::SEMICOLON)) {
        condition = parseExpression();
    } else {
        condition = std::make_unique<BooleanLiteral>(true);
    }
    consume(TokenType::SEMICOLON, "Expected ';' after for condition");

    std::unique_ptr<ASTNode> increment = nullptr;
    if (!check(TokenType::RPAREN)) {
        increment = parseExpression();
    }
    consume(TokenType::RPAREN, "Expected ')' after for clauses");

    consume(TokenType::LBRACE, "Expected '{' after for clauses");
    std::unique_ptr<Block> body = parseBlock();

    std::vector<std::unique_ptr<ASTNode>> whileBodyStatements;
    whileBodyStatements = std::move(body->statements);
    if (increment) {
        whileBodyStatements.push_back(std::move(increment));
    }
    auto whileBody = std::make_unique<Block>(std::move(whileBodyStatements));
    auto whileStmt = std::make_unique<WhileStmt>(std::move(condition), std::move(whileBody));

    if (!initializer) {
        return whileStmt;
    }

    std::vector<std::unique_ptr<ASTNode>> desugaredStatements;
    desugaredStatements.push_back(std::move(initializer));
    desugaredStatements.push_back(std::move(whileStmt));
    return std::make_unique<Block>(std::move(desugaredStatements));
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
    return expr;
}

// Expression parsing with operator precedence
std::unique_ptr<ASTNode> Parser::parseExpression() {
    return parseAssignment();
}

std::unique_ptr<ASTNode> Parser::parseAssignment() {
    std::unique_ptr<ASTNode> expr = parseLogicalOr();
    
    if (check(TokenType::ASSIGN) ||
        check(TokenType::PLUS_ASSIGN) || check(TokenType::MINUS_ASSIGN) ||
        check(TokenType::STAR_ASSIGN) || check(TokenType::SLASH_ASSIGN) ||
        check(TokenType::MOD_ASSIGN) || check(TokenType::AND_ASSIGN) ||
        check(TokenType::OR_ASSIGN) || check(TokenType::XOR_ASSIGN) ||
        check(TokenType::SHL_ASSIGN) || check(TokenType::SHR_ASSIGN)) {
        Token op = advance();
        std::unique_ptr<ASTNode> value = parseAssignment();

        if (auto* ident = dynamic_cast<Identifier*>(expr.get())) {
            std::string name = ident->name;
            if (op.type == TokenType::ASSIGN) {
                return std::make_unique<AssignExpr>(name, std::move(value));
            }

            std::string binaryOp;
            if (op.type == TokenType::PLUS_ASSIGN) binaryOp = "+";
            else if (op.type == TokenType::MINUS_ASSIGN) binaryOp = "-";
            else if (op.type == TokenType::STAR_ASSIGN) binaryOp = "*";
            else if (op.type == TokenType::SLASH_ASSIGN) binaryOp = "/";
            else if (op.type == TokenType::MOD_ASSIGN) binaryOp = "%";
            else if (op.type == TokenType::AND_ASSIGN) binaryOp = "&";
            else if (op.type == TokenType::OR_ASSIGN) binaryOp = "|";
            else if (op.type == TokenType::XOR_ASSIGN) binaryOp = "^";
            else if (op.type == TokenType::SHL_ASSIGN) binaryOp = "<<";
            else if (op.type == TokenType::SHR_ASSIGN) binaryOp = ">>";

            auto combined = std::make_unique<BinaryExpr>(
                std::make_unique<Identifier>(name),
                binaryOp,
                std::move(value)
            );
            return std::make_unique<AssignExpr>(name, std::move(combined));
        }
        
        throw ParseError("Invalid assignment target", op.line, op.column);
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseLogicalOr() {
    std::unique_ptr<ASTNode> left = parseLogicalAnd();

    while (match(TokenType::OR)) {
        Token op = previous();
        std::unique_ptr<ASTNode> right = parseLogicalAnd();
        left = std::make_unique<BinaryExpr>(std::move(left), op.lexeme, std::move(right));
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseLogicalAnd() {
    std::unique_ptr<ASTNode> left = parseBitwiseOr();

    while (match(TokenType::AND)) {
        Token op = previous();
        std::unique_ptr<ASTNode> right = parseBitwiseOr();
        left = std::make_unique<BinaryExpr>(std::move(left), op.lexeme, std::move(right));
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseBitwiseOr() {
    std::unique_ptr<ASTNode> left = parseBitwiseXor();

    while (match(TokenType::BIT_OR)) {
        Token op = previous();
        std::unique_ptr<ASTNode> right = parseBitwiseXor();
        left = std::make_unique<BinaryExpr>(std::move(left), op.lexeme, std::move(right));
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseBitwiseXor() {
    std::unique_ptr<ASTNode> left = parseBitwiseAnd();

    while (match(TokenType::BIT_XOR)) {
        Token op = previous();
        std::unique_ptr<ASTNode> right = parseBitwiseAnd();
        left = std::make_unique<BinaryExpr>(std::move(left), op.lexeme, std::move(right));
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseBitwiseAnd() {
    std::unique_ptr<ASTNode> left = parseEquality();

    while (match(TokenType::BIT_AND)) {
        Token op = previous();
        std::unique_ptr<ASTNode> right = parseEquality();
        left = std::make_unique<BinaryExpr>(std::move(left), op.lexeme, std::move(right));
    }

    return left;
}

std::unique_ptr<ASTNode> Parser::parseEquality() {
    std::unique_ptr<ASTNode> left = parseComparison();
    
    while (check(TokenType::EQ) || check(TokenType::NEQ)) {
        Token op = advance();
        std::unique_ptr<ASTNode> right = parseComparison();
        left = std::make_unique<BinaryExpr>(std::move(left), op.lexeme, std::move(right));
    }
    
    return left;
}

std::unique_ptr<ASTNode> Parser::parseComparison() {
    std::unique_ptr<ASTNode> left = parseShift();
    
    while (check(TokenType::LT) || check(TokenType::LE) ||
           check(TokenType::GT) || check(TokenType::GE)) {
        Token op = advance();
        std::unique_ptr<ASTNode> right = parseShift();
        left = std::make_unique<BinaryExpr>(std::move(left), op.lexeme, std::move(right));
    }
    
    return left;
}

std::unique_ptr<ASTNode> Parser::parseShift() {
    std::unique_ptr<ASTNode> left = parseTerm();

    while (check(TokenType::SHL) || check(TokenType::SHR)) {
        Token op = advance();
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
    
    while (check(TokenType::STAR) || check(TokenType::SLASH) || check(TokenType::MODULO)) {
        Token op = advance();
        std::unique_ptr<ASTNode> right = parseUnary();
        left = std::make_unique<BinaryExpr>(std::move(left), op.lexeme, std::move(right));
    }
    
    return left;
}

std::unique_ptr<ASTNode> Parser::parseUnary() {
    if (check(TokenType::NOT) || check(TokenType::MINUS) || check(TokenType::BIT_NOT)) {
        Token op = advance();
        std::unique_ptr<ASTNode> operand = parseUnary();
        return std::make_unique<UnaryExpr>(op.lexeme, std::move(operand));
    }

    if (check(TokenType::INC) || check(TokenType::DEC)) {
        Token op = advance();
        std::unique_ptr<ASTNode> target = parseUnary();
        auto* ident = dynamic_cast<Identifier*>(target.get());
        if (!ident) {
            throw ParseError("Increment/decrement target must be an identifier", op.line, op.column);
        }
        std::string name = ident->name;
        std::unique_ptr<ASTNode> one = std::make_unique<NumberLiteral>(1);
        std::string binaryOp = (op.type == TokenType::INC) ? "+" : "-";
        std::unique_ptr<ASTNode> rhs = std::make_unique<BinaryExpr>(
            std::make_unique<Identifier>(name),
            binaryOp,
            std::move(one)
        );
        return std::make_unique<AssignExpr>(name, std::move(rhs));
    }

    return parsePostfix();
}

std::unique_ptr<ASTNode> Parser::parsePostfix() {
    std::unique_ptr<ASTNode> expr = parsePrimary();

    if (check(TokenType::INC) || check(TokenType::DEC)) {
        Token op = advance();
        auto* ident = dynamic_cast<Identifier*>(expr.get());
        if (!ident) {
            throw ParseError("Increment/decrement target must be an identifier", op.line, op.column);
        }
        std::string name = ident->name;
        std::unique_ptr<ASTNode> one = std::make_unique<NumberLiteral>(1);
        std::string binaryOp = (op.type == TokenType::INC) ? "+" : "-";
        std::unique_ptr<ASTNode> rhs = std::make_unique<BinaryExpr>(
            std::make_unique<Identifier>(name),
            binaryOp,
            std::move(one)
        );
        return std::make_unique<AssignExpr>(name, std::move(rhs));
    }

    return expr;
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