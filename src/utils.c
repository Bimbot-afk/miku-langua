//
// Created by emar0 on 28/09/2026.
//

#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

char* read_file(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo \"%s\".\n", path);
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(file_size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Error: Memoria insuficiente para leer \"%s\".\n", path);
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    buffer[bytes_read] = '\0'; /* Marcamos el final de la cadena */

    fclose(file);
    return buffer;
}