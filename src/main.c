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
        /* Ejemplo por defecto con dos instrucciones 'sing' */
        codigo = "sing 39 + 1;\nsing 100 * 2 / 4;";
    }

    printf("==============================\n");
    printf("   MIKU LANGUAGE INTERPRETER  \n");
    printf("==============================\n");
    printf("Script:\n%s\n------------------------------\n", codigo);

    Lexer lexer;
    lexer_init(&lexer, codigo);

    Token token = lexer_next_token(&lexer);

    /* Bucle principal: procesa una sentencia 'sing <expresion>;' tras otra */
    while (token.type != TOKEN_EOF) {

        /* 1. Cada instrucción debe comenzar con 'sing' */
        if (token.type != TOKEN_SING) {
            fprintf(stderr, "Error sintactico [Linea %d]: Se esperaba la palabra clave 'sing'.\n", token.line);
            if (fuente_archivo) free(fuente_archivo);
            return 1;
        }

        /* 2. Tras 'sing', debe venir el primer número */
        token = lexer_next_token(&lexer);
        if (token.type != TOKEN_NUMBER) {
            fprintf(stderr, "Error sintactico [Linea %d]: Se esperaba un numero despues de 'sing'.\n", token.line);
            if (fuente_archivo) free(fuente_archivo);
            return 1;
        }

        long acumulador = strtol(token.lexeme, NULL, 10);
        token = lexer_next_token(&lexer);

        /* 3. Evaluamos la cadena de operaciones (+, -, *, /) */
        while (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS ||
               token.type == TOKEN_STAR || token.type == TOKEN_SLASH) {

            TokenType op = token.type;
            Token num_tok = lexer_next_token(&lexer);

            if (num_tok.type != TOKEN_NUMBER) {
                fprintf(stderr, "Error sintactico [Linea %d]: Se esperaba un numero despues del operador.\n", num_tok.line);
                if (fuente_archivo) free(fuente_archivo);
                return 1;
            }

            long valor = strtol(num_tok.lexeme, NULL, 10);

            if (op == TOKEN_PLUS)  acumulador += valor;
            if (op == TOKEN_MINUS) acumulador -= valor;
            if (op == TOKEN_STAR)  acumulador *= valor;
            if (op == TOKEN_SLASH) {
                if (valor == 0) {
                    fprintf(stderr, "Error matematico [Linea %d]: Division por cero.\n", num_tok.line);
                    if (fuente_archivo) free(fuente_archivo);
                    return 1;
                }
                acumulador /= valor;
            }

            token = lexer_next_token(&lexer);
        }

        /* 4. Cada instrucción DEBE terminar con ';' */
        if (token.type != TOKEN_SEMICOLON) {
            fprintf(stderr, "Error sintactico [Linea %d]: Falta ';' al final de la instruccion.\n", token.line);
            if (fuente_archivo) free(fuente_archivo);
            return 1;
        }

        /* 5. 'sing' imprime el resultado obtenido */
        printf("[Miku Sings] => %ld\n", acumulador);

        /* Avanzamos al siguiente token para la siguiente instrucción */
        token = lexer_next_token(&lexer);
    }

    if (fuente_archivo) free(fuente_archivo);
    return 0;
}