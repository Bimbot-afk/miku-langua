#include "lexer.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_VARS 64

typedef struct {
    char name[32];
    long value;
} Variable;

static Variable vars[MAX_VARS];
static int var_count = 0;

static void error_sintactico(int linea, const char *mensaje) {
    fprintf(stderr, "Error sintactico [Linea %d]: %s\n", linea, mensaje);
    exit(1);
}

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

static bool resolve_value(Token token, long *val) {
    if (token.type == TOKEN_NUMBER) {
        *val = strtol(token.lexeme, NULL, 10);
        return true;
    } else if (token.type == TOKEN_IDENTIFIER) {
        if (!get_variable(token.lexeme, token.length, val)) {
            fprintf(stderr, "Error: Variable '%.*s' no definida.\n", token.length, token.lexeme);
            exit(1);
        }
        return true;
    }
    return false;
}

static void jump_instructions(Lexer *lexer) {
    Token t = lexer_next_token(lexer);

    if (t.type == TOKEN_LBRACE) {
        int i = 1; /* Debe empezar en 1 para que el bucle while funcione */
        while (i > 0) {
            t = lexer_next_token(lexer);
            if (t.type == TOKEN_LBRACE) i++;
            else if (t.type == TOKEN_RBRACE) i--;
            else if (t.type == TOKEN_EOF) { /* Doble == para comparar */
                fprintf(stderr, "ERROR: MISSING '}'\n");
                exit(1);
            }
        }
    }
    else {
        while (t.type != TOKEN_SEMICOLON && t.type != TOKEN_EOF) {
            t = lexer_next_token(lexer);
        }
    }
}

static bool ejecutar_instruccion(Lexer *lexer) {
    Token token = lexer_next_token(lexer);
    if (token.type == TOKEN_EOF) return false;

    /* 0. BLOQUES { } */
    if (token.type == TOKEN_LBRACE) {
        while (true) {
            Lexer temp_lexer = *lexer;
            Token future_tok = lexer_next_token(&temp_lexer);

            if (future_tok.type == TOKEN_RBRACE) {
                *lexer = temp_lexer; /* Minúscula, apuntando a tu variable real */
                break;
            } else if (future_tok.type == TOKEN_EOF) {
                error_sintactico(future_tok.line, "Expected '}' to close the block.");
            }

            ejecutar_instruccion(lexer);
        }
        return true;
    }

    /* 1. NOTE */
    if (token.type == TOKEN_NOTE) {
        Token var_name = lexer_next_token(lexer);
        if (var_name.type != TOKEN_IDENTIFIER) error_sintactico(var_name.line, "Se esperaba el nombre de la variable.");
        Token eq = lexer_next_token(lexer);
        if (eq.type != TOKEN_EQUAL) error_sintactico(eq.line, "Se esperaba '='.");

        Token val_tok = lexer_next_token(lexer);
        long valor = 0;

        if (val_tok.type == TOKEN_GACHA) {
            Token max_tok = lexer_next_token(lexer);
            long max_val;
            if (!resolve_value(max_tok, &max_val) || max_val < 0) error_sintactico(max_tok.line, "Maximo invalido para gacha.");
            valor = rand() % (max_val + 1);
        } else if (!resolve_value(val_tok, &valor)) {
            error_sintactico(val_tok.line, "Se esperaba un valor valido.");
        }

        token = lexer_next_token(lexer);
        while (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS || token.type == TOKEN_STAR || token.type == TOKEN_SLASH || token.type == TOKEN_MODULE) {
            TokenType op = token.type;
            Token next_val_tok = lexer_next_token(lexer);
            long next_val;
            if (!resolve_value(next_val_tok, &next_val)) error_sintactico(next_val_tok.line, "Valor invalido.");

            if (op == TOKEN_PLUS)  valor += next_val;
            if (op == TOKEN_MINUS) valor -= next_val;
            if (op == TOKEN_STAR)  valor *= next_val;
            if (op == TOKEN_SLASH) {
                if (next_val == 0) error_sintactico(next_val_tok.line, "Division por cero.");
                valor /= next_val;
            }
            if (op == TOKEN_MODULE) {
                if (next_val == 0) error_sintactico(next_val_tok.line, "Modulo por cero.");
                valor %= next_val;
            }

            token = lexer_next_token(lexer);
        }

        if (token.type != TOKEN_SEMICOLON) error_sintactico(token.line, "Falta ';' al final de 'note'.");
        set_variable(var_name.lexeme, var_name.length, valor);
        return true;
    }

    /* 2. TALK */
    if (token.type == TOKEN_TALK) {
        token = lexer_next_token(lexer);
        if (token.type != TOKEN_STRING) error_sintactico(token.line, "Se esperaba cadena entre comillas.");
        printf("[Miku Talks] => %.*s\n", token.length - 2, token.lexeme + 1);
        token = lexer_next_token(lexer);
        if (token.type != TOKEN_SEMICOLON) error_sintactico(token.line, "Falta ';' al final de 'talk'.");
        return true;
    }

    /* 3. SING */
    if (token.type == TOKEN_SING) {
        token = lexer_next_token(lexer);
        long acumulador;
        if (!resolve_value(token, &acumulador)) error_sintactico(token.line, "Se esperaba numero o variable.");

        token = lexer_next_token(lexer);
        while (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS || token.type == TOKEN_STAR || token.type == TOKEN_SLASH || token.type == TOKEN_MODULE) {
            TokenType op = token.type;
            Token next_val_tok = lexer_next_token(lexer);
            long valor;
            if (!resolve_value(next_val_tok, &valor)) error_sintactico(next_val_tok.line, "Valor invalido.");

            if (op == TOKEN_PLUS)  acumulador += valor;
            if (op == TOKEN_MINUS) acumulador -= valor;
            if (op == TOKEN_STAR)  acumulador *= valor;
            if (op == TOKEN_SLASH) {
                if (valor == 0) error_sintactico(next_val_tok.line, "Division por cero.");
                acumulador /= valor;
            }
            if (op == TOKEN_MODULE) {
                if (valor == 0) error_sintactico(next_val_tok.line, "Modulo por cero.");
                acumulador %= valor;
            }
            token = lexer_next_token(lexer);
        }

        if (token.type != TOKEN_SEMICOLON) error_sintactico(token.line, "Falta ';' al final de 'sing'.");
        printf("[Miku Sings] => %ld\n", acumulador);
        return true;
    }

    /* 4. LISTEN */
    if (token.type == TOKEN_LISTEN) {
        Token var_name = lexer_next_token(lexer);
        if (var_name.type != TOKEN_IDENTIFIER) error_sintactico(var_name.line, "Se esperaba variable.");
        token = lexer_next_token(lexer);
        if (token.type != TOKEN_SEMICOLON) error_sintactico(token.line, "Falta ';'.");

        long input_val = 0;
        printf("[Miku Listens] => ");
        if (scanf("%ld", &input_val) != 1) {
            input_val = 0;
            int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
        }
        set_variable(var_name.lexeme, var_name.length, input_val);
        return true;
    }

    /* 5. CHECK (Condicional) */
    if (token.type == TOKEN_CHECK) {
        Token left_tok = lexer_next_token(lexer);
        long left_val;
        if (!resolve_value(left_tok, &left_val)) error_sintactico(left_tok.line, "Condicion invalida.");

        Token op_tok = lexer_next_token(lexer);
        if (op_tok.type != TOKEN_EQUAL_EQUAL && op_tok.type != TOKEN_NOT_EQUAL &&
            op_tok.type != TOKEN_LESS && op_tok.type != TOKEN_GREATER) {
            error_sintactico(op_tok.line, "Se esperaba ==, !=, < o >");
            }

        Token right_tok = lexer_next_token(lexer);
        long right_val;
        if (!resolve_value(right_tok, &right_val)) error_sintactico(right_tok.line, "Condicion invalida.");

        bool condicion = false;
        if (op_tok.type == TOKEN_EQUAL_EQUAL) condicion = (left_val == right_val);
        else if (op_tok.type == TOKEN_NOT_EQUAL) condicion = (left_val != right_val);
        else if (op_tok.type == TOKEN_LESS) condicion = (left_val < right_val);
        else if (op_tok.type == TOKEN_GREATER) condicion = (left_val > right_val);

        if (condicion) {
            ejecutar_instruccion(lexer);
        } else {
            jump_instructions(lexer);
        }

        return true;
    }

    /* 6. ENCORE (Bucle) */
    if (token.type == TOKEN_ENCORE) {
        Token count_tok = lexer_next_token(lexer);
        long count;
        if (!resolve_value(count_tok, &count)) error_sintactico(count_tok.line, "Repeticiones invalidas.");

        Lexer start_state = *lexer;

        if (count <= 0) {
            Token t = lexer_next_token(lexer);
            while (t.type != TOKEN_SEMICOLON && t.type != TOKEN_EOF) t = lexer_next_token(lexer);
            return true;
        }

        for (long i = 0; i < count; i++) {
            Lexer temp_lexer = start_state;
            ejecutar_instruccion(&temp_lexer);
            if (i == count - 1) {
                *lexer = temp_lexer;
            }
        }
        return true;
    }

    /* Corregida la coma faltante en los argumentos de la funcion */
    error_sintactico(token.line, "Comando no reconocido o caracter invalido.");
    return false;
}

    int main(int argc, char *argv[]) {
        char *fuente_archivo = NULL;
        const char *codigo = NULL;

        srand((unsigned int)time(NULL));

        if (argc >= 2) {
            fuente_archivo = read_file(argv[1]);
            if (fuente_archivo == NULL) return 1;
            codigo = fuente_archivo;
        } else {
            codigo = "note iter = 1;\nencore 3 talk \"Hola!\";";
        }

        printf("==============================\n");
        printf("   MIKU LANGUAGE INTERPRETER  \n");
        printf("==============================\n");

        Lexer lexer;
        lexer_init(&lexer, codigo);

        while (ejecutar_instruccion(&lexer)) {
        }

        if (fuente_archivo) free(fuente_archivo);
        printf("==============================\n");
        printf("   EXECUTION END     \n");
        printf("==============================\n");
        return 0;
    }