//
// Created by emar0 on 28/09/2026.
//

#include "../include/lexer.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    const char *codigo;

    if (argc >= 2) {
        codigo = argv[1];
    } else {
        codigo = "10 + 25 + 3";
    }
    printf("thinking: %s\n", codigo);

    Lexer lexer;
    lexer_init(&lexer, codigo);

    Token token;
    do {
        token = lexer_next_token(&lexer);

        if (token.type == TOKEN_NUMBER) {
            printf("[NUMERO] %.*s\n", token.length, token.lexeme);
        } else if (token.type == TOKEN_PLUS) {
            printf("[SUMA]   +\n");
        } else if (token.type == TOKEN_EOF) {
            printf("[FIN]\n");
        } else {
            printf("[ERROR] Carácter no reconocido\n");
        }

    } while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR);

    return 0;
}