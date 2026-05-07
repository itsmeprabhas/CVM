#include "lexer.hpp"
#include <stdexcept>
#include <cctype>
#include <iostream>

Lexer::Lexer(const std::string& source) 
    : source(source), pos(0), line(1), column(1) {}

char Lexer::current() {
    if (isAtEnd()) return '\0';
    return source[pos];
}

char Lexer::peek() {
    if (pos + 1 >= source.size()) return '\0';
    return source[pos + 1];
}

char Lexer::advance() {
    char c = current();
    pos++;
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

bool Lexer::isAtEnd() {
    return pos >= source.size();
}

bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

void Lexer::skipWhitespace() {
    while (!isAtEnd() && std::isspace(current())) {
        advance();
    }
}

Token Lexer::makeToken(TokenType type) {
    return Token(type, "", line, column);
}

Token Lexer::scanNumber() {
    int startCol = column - 1;
    size_t startPos = pos;
    
    while (!isAtEnd() && isDigit(current())) {
        advance();
    }
    
    std::string numStr = source.substr(startPos, pos - startPos);
    return Token(TokenType::NUMBER, numStr, line, startCol);
}

Token Lexer::scanIdentifierOrKeyword() {
    int startCol = column - 1;
    size_t startPos = pos;
    
    while (!isAtEnd() && isAlphaNumeric(current())) {
        advance();
    }
    
    std::string text = source.substr(startPos, pos - startPos);
    
    // Check for keywords
    TokenType type = TokenType::IDENTIFIER;
    if (text == "let")      type = TokenType::LET;
    else if (text == "print")   type = TokenType::PRINT;
    else if (text == "input")   type = TokenType::INPUT;
    else if (text == "if")      type = TokenType::IF;
    else if (text == "else")    type = TokenType::ELSE;
    else if (text == "while")   type = TokenType::WHILE;
    else if (text == "true" || text == "false") type = TokenType::BOOLEAN;
    
    return Token(type, text, line, startCol);
}

Token Lexer::scanToken() {
    skipWhitespace();
    
    if (isAtEnd()) {
        return Token(TokenType::EOF_TOKEN, "", line, column);
    }
    
    char c = current();
    int startCol = column;
    
    // Single character tokens
    switch (c) {
        case '+': advance(); return Token(TokenType::PLUS, "+", line, startCol);
        case '-': advance(); return Token(TokenType::MINUS, "-", line, startCol);
        case '*': advance(); return Token(TokenType::STAR, "*", line, startCol);
        case '/': advance(); return Token(TokenType::SLASH, "/", line, startCol);
        case '(': advance(); return Token(TokenType::LPAREN, "(", line, startCol);
        case ')': advance(); return Token(TokenType::RPAREN, ")", line, startCol);
        case '{': advance(); return Token(TokenType::LBRACE, "{", line, startCol);
        case '}': advance(); return Token(TokenType::RBRACE, "}", line, startCol);
        case ';': advance(); return Token(TokenType::SEMICOLON, ";", line, startCol);
    }
    
    // Two character tokens
    if (c == '=' && peek() == '=') {
        advance(); advance();
        return Token(TokenType::EQ, "==", line, startCol);
    }
    if (c == '=' && peek() != '=') {
        advance();
        return Token(TokenType::ASSIGN, "=", line, startCol);
    }
    if (c == '<') {
        advance();
        return Token(TokenType::LT, "<", line, startCol);
    }
    
    // Numbers
    if (isDigit(c)) {
        return scanNumber();
    }
    
    // Identifiers and keywords
    if (isAlpha(c)) {
        return scanIdentifierOrKeyword();
    }
    
    // Unknown token
    std::string err = "Unexpected character: '";
    err += c;
    err += "'";
    advance();
    throw LexError(err, line, startCol);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (!isAtEnd()) {
        try {
            Token token = scanToken();
            if (token.type != TokenType::EOF_TOKEN) {
                tokens.push_back(token);
            }
        } catch (const LexError& e) {
            std::cerr << "[Lex Error] Line " << e.line << ":" << e.column 
                      << " - " << e.what() << std::endl;
            throw;
        }
    }
    
    tokens.push_back(Token(TokenType::EOF_TOKEN, "", line, column));
    return tokens;
}