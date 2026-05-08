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
  FOR,   // for keyword
  AND,   // and keyword
  OR,    // or keyword
  NOT,   // not keyword

  // Operators
  PLUS,       // +
  MINUS,      // -
  STAR,       // *
  SLASH,      // /
  MODULO,     // %
  EQ,         // ==
  NEQ,        // !=
  LE,         // <=
  LT,         // <
  GE,         // >=
  GT,         // >
  SHL,        // <<
  SHR,        // >>
  BIT_AND,    // &
  BIT_OR,     // |
  BIT_XOR,    // ^
  BIT_NOT,    // ~
  INC,        // ++
  DEC,        // --
  PLUS_ASSIGN,   // +=
  MINUS_ASSIGN,  // -=
  STAR_ASSIGN,   // *=
  SLASH_ASSIGN,  // /=
  MOD_ASSIGN,    // %=
  AND_ASSIGN,    // &=
  OR_ASSIGN,     // |=
  XOR_ASSIGN,    // ^=
  SHL_ASSIGN,    // <<=
  SHR_ASSIGN,    // >>=
  ASSIGN,     // =

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