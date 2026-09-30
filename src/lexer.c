#include  "lexer.h"
#include <ctype.h>

void lexer_init(Lexer *lexer, const char *source) {
    lexer -> start = source;
    lexer -> current = source;
    lexer -> line =1;
}

static bool is_at_end(const Lexer *lexer) {
    return *lexer->current == '\0';
}

static char advance(Lexer *lexer) {
    lexer->current++;
    return *lexer->current;
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

Token lexer_next_token(Lexer *lexer) {
    skip_whitespace(lexer);

    lexer->start = lexer->current;

    if (is_at_end(lexer)) {
        return make_token(lexer, TOKEN_EOF);
    }

    char c = advance(lexer);

    if (isdigit(c)) {
        while (isdigit(peek(lexer))) {
            advance(lexer);
        }
        return make_token(lexer);
    }
    if (c == '+') {
        return make_token(lexer, TOKEN_PLUS);
    }
    return make_token(lexer, TOKEN_ERROR);
}


