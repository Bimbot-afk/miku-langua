#include "lexer.h"
#include <ctype.h>
#include <string.h>

void lexer_init(Lexer *lexer, const char *source) {
    lexer->start = source;
    lexer->current = source;
    lexer->line = 1;
}

static bool is_at_end(const Lexer *lexer) {
    return *lexer->current == '\0';
}

static char advance(Lexer *lexer) {
    lexer->current++;
    return lexer->current[-1];
}

static char peek(const Lexer *lexer) {
    return *lexer->current;
}

static Token make_token(Lexer *lexer, TokenType type) {
    Token token;
    token.type = type;
    token.lexeme = lexer->start;
    token.length = (int)(lexer->current - lexer->start);
    token.line = lexer->line;
    token.column = 0;
    return token;
}

static void skip_whitespace(Lexer *lexer) {
    while (!is_at_end(lexer)) {
        char const c = peek(lexer);
        if (c == ' ' || c == '\t' || c == '\r') {
            advance(lexer);
        } else if (c == '\n') {
            lexer->line++;
            advance(lexer);
        } else {
            break;
        }
    }
}

/* Escanea una palabra completa y revisa si es la palabra reservada 'sing' */
static Token scan_identifier(Lexer *lexer) {
    while (isalnum(peek(lexer)) || peek(lexer) == '_') {
        advance(lexer);
    }

    int length = (int)(lexer->current - lexer->start);

    /* Si mide 4 caracteres y coincide con "sing" */
    if (length == 4 && memcmp(lexer->start, "sing", 4) == 0) {
        return make_token(lexer, TOKEN_SING);
    }

    return make_token(lexer, TOKEN_IDENTIFIER);
}

Token lexer_next_token(Lexer *lexer) {
    skip_whitespace(lexer);

    lexer->start = lexer->current;

    if (is_at_end(lexer)) {
        return make_token(lexer, TOKEN_EOF);
    }

    char c = advance(lexer);

    /* 1. ¿Es una letra o identificador/keyword? */
    if (isalpha(c) || c == '_') {
        return scan_identifier(lexer);
    }

    /* 2. ¿Es un número? */
    if (isdigit(c)) {
        while (isdigit(peek(lexer))) {
            advance(lexer);
        }
        return make_token(lexer, TOKEN_NUMBER);
    }

    /* 3. Operadores y delimitadores */
    if (c == '+') return make_token(lexer, TOKEN_PLUS);
    if (c == '-') return make_token(lexer, TOKEN_MINUS);
    if (c == '*') return make_token(lexer, TOKEN_STAR); /* Asegúrate de usar TOKEN_STAR o TOKEN_MUL según tu token.h */
    if (c == '/') return make_token(lexer, TOKEN_SLASH); /* o TOKEN_DIV según tu token.h */
    if (c == ';') return make_token(lexer, TOKEN_SEMICOLON);

    return make_token(lexer, TOKEN_ERROR);
}