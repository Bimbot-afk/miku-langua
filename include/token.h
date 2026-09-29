//
// Created by emar0 on 28/09/2026.
//

#ifndef MIKU_LANGUAGE_TOKEN_H
#define MIKU_LANGUAGE_TOKEN_H

#include "common.h"

typedef enum {
    TOKEN_ERROR,
    TOKEN_EOF,

    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,

    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_BANG_EQUAL,

    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_NUMBER,

    TOKEN_LET,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_FN,

    TOKEN_FOR,
    TOKEN_WHILE,

    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_RETURN,

    TOKEN_LESS,
    TOKEN_GREATER

} TokenType;

typedef struct {
    TokenType type;
    const char *lexeme;
    int length;
    int line;
    int column;
} Token;

const char *tokenTypeToString(TokenType type);

#endif //MIKU_LANGUAGE_TOKEN_H
