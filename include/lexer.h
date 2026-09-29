#ifndef MIKU_LANGUAGE_LEXER_H
#define MIKU_LANGUAGE_LEXER_H

#include "token.h"

typedef struct {
    const char *start;
    const char *current;
    int line;
} Lexer;

void lexer_init(Lexer *lexer, const char *source);
Token lexer_nextToken(Lexer *lexer);

#endif //MIKU_LANGUAGE_LEXER_H
