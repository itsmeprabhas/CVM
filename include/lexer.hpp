#ifndef LEXER_HPP
#define LEXER_HPP

#include "token.hpp"
#include <string>
#include <vector>

class Lexer {
private:
    std::string source;
    size_t pos;
    int line;
    int column;
    
    char current();
    char peek();
    char advance();
    void skipWhitespace();
    void skipLineComment();
    void skipBlockComment();
    bool isAtEnd();
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    
    Token scanNumber();
    Token scanIdentifierOrKeyword();
    Token makeToken(TokenType type);
    
public:
    Lexer(const std::string& source);
    
    std::vector<Token> tokenize();
    Token scanToken();
};

// Exception for lexical errors
class LexError : public std::runtime_error {
public:
    int line;
    int column;
    LexError(const std::string& msg, int ln, int col)
        : std::runtime_error(msg), line(ln), column(col) {}
};

#endif // LEXER_HPP