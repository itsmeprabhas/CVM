#include "token.hpp"
#include <sstream>

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::NUMBER:      return "NUMBER";
        case TokenType::BOOLEAN:     return "BOOLEAN";
        case TokenType::IDENTIFIER:  return "IDENTIFIER";
        case TokenType::LET:         return "LET";
        case TokenType::PRINT:       return "PRINT";
        case TokenType::INPUT:       return "INPUT";
        case TokenType::IF:          return "IF";
        case TokenType::ELSE:        return "ELSE";
        case TokenType::WHILE:       return "WHILE";
        case TokenType::PLUS:        return "PLUS";
        case TokenType::MINUS:       return "MINUS";
        case TokenType::STAR:        return "STAR";
        case TokenType::SLASH:       return "SLASH";
        case TokenType::EQ:          return "EQ";
        case TokenType::LT:          return "LT";
        case TokenType::ASSIGN:      return "ASSIGN";
        case TokenType::LPAREN:      return "LPAREN";
        case TokenType::RPAREN:      return "RPAREN";
        case TokenType::LBRACE:      return "LBRACE";
        case TokenType::RBRACE:      return "RBRACE";
        case TokenType::SEMICOLON:   return "SEMICOLON";
        case TokenType::EOF_TOKEN:   return "EOF";
        case TokenType::UNKNOWN:     return "UNKNOWN";
        default:                     return "UNDEFINED";
    }
}

std::string Token::toString() const {
    std::ostringstream oss;
    oss << "[" << tokenTypeToString(type) << " '" << lexeme 
        << "' at line " << line << ":" << column << "]";
    return oss.str();
}