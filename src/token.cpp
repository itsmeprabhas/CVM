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
        case TokenType::FOR:         return "FOR";
        case TokenType::AND:         return "AND";
        case TokenType::OR:          return "OR";
        case TokenType::NOT:         return "NOT";
        case TokenType::PLUS:        return "PLUS";
        case TokenType::MINUS:       return "MINUS";
        case TokenType::STAR:        return "STAR";
        case TokenType::SLASH:       return "SLASH";
        case TokenType::MODULO:      return "MODULO";
        case TokenType::EQ:          return "EQ";
        case TokenType::NEQ:         return "NEQ";
        case TokenType::LE:          return "LE";
        case TokenType::LT:          return "LT";
        case TokenType::GE:          return "GE";
        case TokenType::GT:          return "GT";
        case TokenType::SHL:         return "SHL";
        case TokenType::SHR:         return "SHR";
        case TokenType::BIT_AND:     return "BIT_AND";
        case TokenType::BIT_OR:      return "BIT_OR";
        case TokenType::BIT_XOR:     return "BIT_XOR";
        case TokenType::BIT_NOT:     return "BIT_NOT";
        case TokenType::INC:         return "INC";
        case TokenType::DEC:         return "DEC";
        case TokenType::PLUS_ASSIGN: return "PLUS_ASSIGN";
        case TokenType::MINUS_ASSIGN:return "MINUS_ASSIGN";
        case TokenType::STAR_ASSIGN: return "STAR_ASSIGN";
        case TokenType::SLASH_ASSIGN:return "SLASH_ASSIGN";
        case TokenType::MOD_ASSIGN:  return "MOD_ASSIGN";
        case TokenType::AND_ASSIGN:  return "AND_ASSIGN";
        case TokenType::OR_ASSIGN:   return "OR_ASSIGN";
        case TokenType::XOR_ASSIGN:  return "XOR_ASSIGN";
        case TokenType::SHL_ASSIGN:  return "SHL_ASSIGN";
        case TokenType::SHR_ASSIGN:  return "SHR_ASSIGN";
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