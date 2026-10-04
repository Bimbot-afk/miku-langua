#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    const char *codigo;

    if (argc >= 2) {
        codigo = argv[1];
    } else {
        codigo = "10 + 25 + 3";
    }

    printf("Expresion: \"%s\"\n", codigo);

    Lexer lexer;
    lexer_init(&lexer, codigo);


    Token token = lexer_next_token(&lexer);

    if (token.type != TOKEN_NUMBER) {
        fprintf(stderr, "Error de sintaxis: se esperaba un numero inicial.\n");
        return 1;
    }

    long resultado = strtol(token.lexeme, NULL, 10);

    token = lexer_next_token(&lexer);

    while (token.type == TOKEN_PLUS) {

        Token num_token = lexer_next_token(&lexer);

        if (num_token.type != TOKEN_NUMBER) {
            fprintf(stderr, "Error de sintaxis: se esperaba un numero despues del '+'.\n");
            return 1;
        }

        long valor = strtol(num_token.lexeme, NULL, 10);
        resultado += valor;


        token = lexer_next_token(&lexer);
    }

    if (token.type != TOKEN_EOF) {
        fprintf(stderr, "Error de sintaxis: token inesperado en la expresion.\n");
        return 1;
    }


    printf("Resultado = %ld\n", resultado);

    return 0;
}