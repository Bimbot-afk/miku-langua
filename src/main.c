#include "lexer.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_VARS 64

typedef struct {
    char name[32];
    long value;
} Variable;

static Variable vars[MAX_VARS];
static int var_count = 0;

static void set_variable(const char *name, int length, long value) {
    for (int i = 0; i < var_count; i++) {
        if (strncmp(vars[i].name, name, length) == 0 && vars[i].name[length] == '\0') {
            vars[i].value = value;
            return;
        }
    }
    if (var_count < MAX_VARS) {
        strncpy(vars[var_count].name, name, length);
        vars[var_count].name[length] = '\0';
        vars[var_count].value = value;
        var_count++;
    }
}

static bool get_variable(const char *name, int length, long *out_value) {
    for (int i = 0; i < var_count; i++) {
        if (strncmp(vars[i].name, name, length) == 0 && vars[i].name[length] == '\0') {
            *out_value = vars[i].value;
            return true;
        }
    }
    return false;
}

/* Resuelve si el token es un número directo o el valor de una variable */
static bool resolve_value(Token token, long *val) {
    if (token.type == TOKEN_NUMBER) {
        *val = strtol(token.lexeme, NULL, 10);
        return true;
    } else if (token.type == TOKEN_IDENTIFIER) {
        if (!get_variable(token.lexeme, token.length, val)) {
            fprintf(stderr, "Error semantico [Linea %d]: Variable '%.*s' no definida.\n",
                    token.line, token.length, token.lexeme);
            return false;
        }
        return true;
    }
    return false;
}

int main(int argc, char *argv[]) {
    char *fuente_archivo = NULL;
    const char *codigo = NULL;

    if (argc >= 2) {
        fuente_archivo = read_file(argv[1]);
        if (fuente_archivo == NULL) return 1;
        codigo = fuente_archivo;
    } else {
        codigo = "note miku = 39;\ncheck miku == 39 talk \"Miku OK!\";\nsing miku + 1;";
    }

    printf("==============================\n");
    printf("   MIKU LANGUAGE INTERPRETER  \n");
    printf("==============================\n");

    Lexer lexer;
    lexer_init(&lexer, codigo);

    Token token = lexer_next_token(&lexer);

    while (token.type != TOKEN_EOF) {

        /* 1. COMANDO NOTE: note <id> = <expr>; */
        if (token.type == TOKEN_NOTE) {
            Token var_name = lexer_next_token(&lexer);
            if (var_name.type != TOKEN_IDENTIFIER) {
                fprintf(stderr, "Error sintactico [Linea %d]: Se esperaba el nombre de la variable.\n", var_name.line);
                if (fuente_archivo) free(fuente_archivo);
                return 1;
            }

            Token eq = lexer_next_token(&lexer);
            if (eq.type != TOKEN_EQUAL) {
                fprintf(stderr, "Error sintactico [Linea %d]: Se esperaba '=' despues de la variable.\n", eq.line);
                if (fuente_archivo) free(fuente_archivo);
                return 1;
            }

            Token val_tok = lexer_next_token(&lexer);
            long valor;
            if (!resolve_value(val_tok, &valor)) {
                fprintf(stderr, "Error sintactico [Linea %d]: Se esperaba un valor o variable valida.\n", val_tok.line);
                if (fuente_archivo) free(fuente_archivo);
                return 1;
            }

            token = lexer_next_token(&lexer);
            while (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS ||
                   token.type == TOKEN_STAR || token.type == TOKEN_SLASH) {
                TokenType op = token.type;
                Token next_val_tok = lexer_next_token(&lexer);
                long next_val;
                if (!resolve_value(next_val_tok, &next_val)) {
                    if (fuente_archivo) free(fuente_archivo);
                    return 1;
                }
                if (op == TOKEN_PLUS)  valor += next_val;
                if (op == TOKEN_MINUS) valor -= next_val;
                if (op == TOKEN_STAR)  valor *= next_val;
                if (op == TOKEN_SLASH) {
                    if (next_val == 0) {
                        fprintf(stderr, "Error matematico [Linea %d]: Division por cero.\n", next_val_tok.line);
                        if (fuente_archivo) free(fuente_archivo);
                        return 1;
                    }
                    valor /= next_val;
                }
                token = lexer_next_token(&lexer);
            }

            if (token.type != TOKEN_SEMICOLON) {
                fprintf(stderr, "Error sintactico [Linea %d]: Falta ';' al final de 'note'.\n", token.line);
                if (fuente_archivo) free(fuente_archivo);
                return 1;
            }

            set_variable(var_name.lexeme, var_name.length, valor);
            token = lexer_next_token(&lexer);
            continue;
        }

        /* 2. COMANDO TALK: talk <string>; */
        if (token.type == TOKEN_TALK) {
            token = lexer_next_token(&lexer);

            if (token.type != TOKEN_STRING) {
                fprintf(stderr, "Error sintactico [Linea %d]: Se esperaba una cadena de texto entre comillas.\n", token.line);
                if (fuente_archivo) free(fuente_archivo);
                return 1;
            }

            printf("[Miku Talks] => %.*s\n", token.length - 2, token.lexeme + 1);

            token = lexer_next_token(&lexer);
            if (token.type != TOKEN_SEMICOLON) {
                fprintf(stderr, "Error sintactico [Linea %d]: Falta ';' al final de 'talk'.\n", token.line);
                if (fuente_archivo) free(fuente_archivo);
                return 1;
            }

            token = lexer_next_token(&lexer);
            continue;
        }

        /* 3. COMANDO SING: sing <expr>; */
        if (token.type == TOKEN_SING) {
            token = lexer_next_token(&lexer);
            long acumulador;
            if (!resolve_value(token, &acumulador)) {
                fprintf(stderr, "Error sintactico [Linea %d]: Se esperaba un numero o variable tras 'sing'.\n", token.line);
                if (fuente_archivo) free(fuente_archivo);
                return 1;
            }

            token = lexer_next_token(&lexer);

            while (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS ||
                   token.type == TOKEN_STAR || token.type == TOKEN_SLASH) {

                TokenType op = token.type;
                Token next_val_tok = lexer_next_token(&lexer);
                long valor;

                if (!resolve_value(next_val_tok, &valor)) {
                    if (fuente_archivo) free(fuente_archivo);
                    return 1;
                }

                if (op == TOKEN_PLUS)  acumulador += valor;
                if (op == TOKEN_MINUS) acumulador -= valor;
                if (op == TOKEN_STAR)  acumulador *= valor;
                if (op == TOKEN_SLASH) {
                    if (valor == 0) {
                        fprintf(stderr, "Error matematico [Linea %d]: Division por cero.\n", next_val_tok.line);
                        if (fuente_archivo) free(fuente_archivo);
                        return 1;
                    }
                    acumulador /= valor;
                }

                token = lexer_next_token(&lexer);
            }

            if (token.type != TOKEN_SEMICOLON) {
                fprintf(stderr, "Error sintactico [Linea %d]: Falta ';' al final de 'sing'.\n", token.line);
                if (fuente_archivo) free(fuente_archivo);
                return 1;
            }

            printf("[Miku Sings] => %ld\n", acumulador);
            token = lexer_next_token(&lexer);
            continue;
        }

        /* 4. COMANDO CHECK: check <val1> (== | !=) <val2> <instruccion> */
        if (token.type == TOKEN_CHECK) {
            Token left_tok = lexer_next_token(&lexer);
            long left_val;
            if (!resolve_value(left_tok, &left_val)) {
                fprintf(stderr, "Error sintactico [Linea %d]: Se esperaba un valor en la condicion.\n", left_tok.line);
                if (fuente_archivo) free(fuente_archivo);
                return 1;
            }

            Token op_tok = lexer_next_token(&lexer);
            if (op_tok.type != TOKEN_EQUAL_EQUAL && op_tok.type != TOKEN_NOT_EQUAL) {
                fprintf(stderr, "Error sintactico [Linea %d]: Se esperaba '==' o '!=' en check.\n", op_tok.line);
                if (fuente_archivo) free(fuente_archivo);
                return 1;
            }

            Token right_tok = lexer_next_token(&lexer);
            long right_val;
            if (!resolve_value(right_tok, &right_val)) {
                fprintf(stderr, "Error sintactico [Linea %d]: Se esperaba un valor tras la comparacion.\n", right_tok.line);
                if (fuente_archivo) free(fuente_archivo);
                return 1;
            }

            bool condicion = false;
            if (op_tok.type == TOKEN_EQUAL_EQUAL) condicion = (left_val == right_val);
            if (op_tok.type == TOKEN_NOT_EQUAL)   condicion = (left_val != right_val);

            token = lexer_next_token(&lexer);

            if (condicion) {
                /* Condición verdadera: ejecuta la instrucción siguiente en la siguiente iteración */
                continue;
            } else {
                /* Condición falsa: descarta tokens hasta terminar la instrucción con ';' */
                while (token.type != TOKEN_SEMICOLON && token.type != TOKEN_EOF) {
                    token = lexer_next_token(&lexer);
                }
                if (token.type == TOKEN_SEMICOLON) {
                    token = lexer_next_token(&lexer);
                }
                continue;
            }
        }

        fprintf(stderr, "Error sintactico [Linea %d]: Comando no reconocido.\n", token.line);
        if (fuente_archivo) free(fuente_archivo);
        return 1;
    }

    if (fuente_archivo) free(fuente_archivo);
    return 0;
}