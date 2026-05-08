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
    while (!isAtEnd()) {
        if (std::isspace(current())) {
            advance();
            continue;
        }

        if (current() == '/' && peek() == '/') {
            skipLineComment();
            continue;
        }

        if (current() == '/' && peek() == '*') {
            skipBlockComment();
            continue;
        }

        break;
    }
}

void Lexer::skipLineComment() {
    while (!isAtEnd() && current() != '\n') {
        advance();
    }
}

void Lexer::skipBlockComment() {
    // Consume opening "/*"
    advance();
    advance();

    while (!isAtEnd()) {
        if (current() == '*' && peek() == '/') {
            // Consume closing "*/"
            advance();
            advance();
            return;
        }
        advance();
    }

    throw LexError("Unterminated block comment", line, column);
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
    else if (text == "for")     type = TokenType::FOR;
    else if (text == "case")    type = TokenType::CASE;
    else if (text == "when")    type = TokenType::WHEN;
    else if (text == "break")   type = TokenType::BREAK;
    else if (text == "continue") type = TokenType::CONTINUE;
    else if (text == "and")     type = TokenType::AND;
    else if (text == "or")      type = TokenType::OR;
    else if (text == "not")     type = TokenType::NOT;
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
        case '+':
            if (peek() == '+') {
                advance(); advance();
                return Token(TokenType::INC, "++", line, startCol);
            }
            if (peek() == '=') {
                advance(); advance();
                return Token(TokenType::PLUS_ASSIGN, "+=", line, startCol);
            }
            advance(); return Token(TokenType::PLUS, "+", line, startCol);
        case '-':
            if (peek() == '-') {
                advance(); advance();
                return Token(TokenType::DEC, "--", line, startCol);
            }
            if (peek() == '=') {
                advance(); advance();
                return Token(TokenType::MINUS_ASSIGN, "-=", line, startCol);
            }
            advance(); return Token(TokenType::MINUS, "-", line, startCol);
        case '*':
            if (peek() == '=') {
                advance(); advance();
                return Token(TokenType::STAR_ASSIGN, "*=", line, startCol);
            }
            advance(); return Token(TokenType::STAR, "*", line, startCol);
        case '/':
            if (peek() == '=') {
                advance(); advance();
                return Token(TokenType::SLASH_ASSIGN, "/=", line, startCol);
            }
            advance(); return Token(TokenType::SLASH, "/", line, startCol);
        case '%':
            if (peek() == '=') {
                advance(); advance();
                return Token(TokenType::MOD_ASSIGN, "%=", line, startCol);
            }
            advance(); return Token(TokenType::MODULO, "%", line, startCol);
        case '(': advance(); return Token(TokenType::LPAREN, "(", line, startCol);
        case ')': advance(); return Token(TokenType::RPAREN, ")", line, startCol);
        case '{': advance(); return Token(TokenType::LBRACE, "{", line, startCol);
        case '}': advance(); return Token(TokenType::RBRACE, "}", line, startCol);
        case ';': advance(); return Token(TokenType::SEMICOLON, ";", line, startCol);
        case '^':
            if (peek() == '=') {
                advance(); advance();
                return Token(TokenType::XOR_ASSIGN, "^=", line, startCol);
            }
            advance(); return Token(TokenType::BIT_XOR, "^", line, startCol);
        case '~':
            advance(); return Token(TokenType::BIT_NOT, "~", line, startCol);
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
    if (c == '!' && peek() == '=') {
        advance(); advance();
        return Token(TokenType::NEQ, "!=", line, startCol);
    }
    if (c == '!') {
        advance();
        return Token(TokenType::NOT, "!", line, startCol);
    }
    if (c == '<' && peek() == '=') {
        advance(); advance();
        return Token(TokenType::LE, "<=", line, startCol);
    }
    if (c == '<' && peek() == '<' && (pos + 2 < source.size()) && source[pos + 2] == '=') {
        advance(); advance(); advance();
        return Token(TokenType::SHL_ASSIGN, "<<=", line, startCol);
    }
    if (c == '<' && peek() == '<') {
        advance(); advance();
        return Token(TokenType::SHL, "<<", line, startCol);
    }
    if (c == '<') {
        advance();
        return Token(TokenType::LT, "<", line, startCol);
    }
    if (c == '>' && peek() == '=') {
        advance(); advance();
        return Token(TokenType::GE, ">=", line, startCol);
    }
    if (c == '>' && peek() == '>' && (pos + 2 < source.size()) && source[pos + 2] == '=') {
        advance(); advance(); advance();
        return Token(TokenType::SHR_ASSIGN, ">>=", line, startCol);
    }
    if (c == '>' && peek() == '>') {
        advance(); advance();
        return Token(TokenType::SHR, ">>", line, startCol);
    }
    if (c == '>') {
        advance();
        return Token(TokenType::GT, ">", line, startCol);
    }
    if (c == '&') {
        if (peek() == '&') {
            advance(); advance();
            return Token(TokenType::AND, "&&", line, startCol);
        }
        if (peek() == '=') {
            advance(); advance();
            return Token(TokenType::AND_ASSIGN, "&=", line, startCol);
        }
        advance();
        return Token(TokenType::BIT_AND, "&", line, startCol);
    }
    if (c == '|') {
        if (peek() == '|') {
            advance(); advance();
            return Token(TokenType::OR, "||", line, startCol);
        }
        if (peek() == '=') {
            advance(); advance();
            return Token(TokenType::OR_ASSIGN, "|=", line, startCol);
        }
        advance();
        return Token(TokenType::BIT_OR, "|", line, startCol);
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