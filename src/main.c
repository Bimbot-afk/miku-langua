#include "lexer.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *fuente_archivo = NULL;
    const char *codigo = NULL;

    if (argc >= 2) {
        fuente_archivo = read_file(argv[1]);
        if (fuente_archivo == NULL) return 1;
        codigo = fuente_archivo;
    } else {
        codigo = "39 + 1 - 10 * 2";
    }

    printf("==============================\n");
    printf("   MIKU LANGUAGE INTERPRETER  \n");
    printf("==============================\n");
    printf("Script:\n%s\n------------------------------\n", codigo);

    Lexer lexer;
    lexer_init(&lexer, codigo);

    Token token = lexer_next_token(&lexer);

    if (token.type != TOKEN_NUMBER) {
        fprintf(stderr, "Error: Se esperaba un numero al inicio.\n");
        if (fuente_archivo) free(fuente_archivo);
        return 1;
    }

    long acumulador = strtol(token.lexeme, NULL, 10);
    token = lexer_next_token(&lexer);

    while (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS ||
           token.type == TOKEN_STAR || token.type == TOKEN_SLASH) {

        TokenType op = token.type;
        Token num_tok = lexer_next_token(&lexer);

        if (num_tok.type != TOKEN_NUMBER) {
            fprintf(stderr, "Error: Se esperaba un numero despues del operador.\n");
            if (fuente_archivo) free(fuente_archivo);
            return 1;
        }

        long valor = strtol(num_tok.lexeme, NULL, 10);

        if (op == TOKEN_PLUS)  acumulador += valor;
        if (op == TOKEN_MINUS) acumulador -= valor;
        if (op == TOKEN_STAR)  acumulador *= valor;
        if (op == TOKEN_SLASH) {
            if (valor == 0) {
                fprintf(stderr, "Error matematico: Division por cero.\n");
                if (fuente_archivo) free(fuente_archivo);
                return 1;
            }
            acumulador /= valor;
        }

        token = lexer_next_token(&lexer);
    }

    if (token.type != TOKEN_EOF) {
        fprintf(stderr, "Error: Token o caracter invalido.\n");
        if (fuente_archivo) free(fuente_archivo);
        return 1;
    }

    printf("[Miku Out] => %ld\n", acumulador);

    if (fuente_archivo) free(fuente_archivo);
    return 0;
}