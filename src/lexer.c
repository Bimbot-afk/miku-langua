#include  "lexer.h"
#include <ctype.h>

void lexer_init(Lexer *lexer, const char *source) {
    lexer -> start = source;
    lexer -> current = source;
    lexer -> line =1;
}

static bool is_at_end(Lexer *lexer) {
    return *lexer->current == '\0';
}


