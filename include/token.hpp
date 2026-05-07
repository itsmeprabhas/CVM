#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <vector>

// Token Types for our language
enum class TokenType {
  // Literals
  NUMBER,  // 0-9
  BOOLEAN, // true, false

  // Identifiers & Keywords
  IDENTIFIER,
  LET,   // let keyword
  PRINT, // print keyword
  INPUT, // input keyword
  IF,    // if keyword
  ELSE,  // else keyword
  WHILE, // while keyword

  // Operators
  PLUS,   // +
  MINUS,  // -
  STAR,   // *
  SLASH,  // /
  EQ,     // ==
  LT,     // <
  ASSIGN, // =

  // Delimiters
  LPAREN,    // (
  RPAREN,    // )
  LBRACE,    // {
  RBRACE,    // }
  SEMICOLON, // ;

  // Special
  EOF_TOKEN,
  UNKNOWN
};

// Token structure
struct Token {
  TokenType type;
  std::string lexeme;
  int line;
  int column;

  Token(TokenType t, const std::string &lex, int ln, int col)
      : type(t), lexeme(lex), line(ln), column(col) {}

  std::string toString() const;
};

// Utility function to convert TokenType to string
std::string tokenTypeToString(TokenType type);

#endif // TOKEN_HPP