#include <stdio.h>
#include <stdlib.h>

#include "../include/file_processing.h"

unsigned char* read_file(const char *file_directory, size_t *file_size) {
    *file_size = 0;
    FILE *file = fopen(file_directory, "rb");

    if (!file) {
        fprintf(stderr, "Error opening input file\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    const size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size == 0) {
        fprintf(stderr, "Input file is empty\n");
        fclose(file);
        return NULL;
    }

    unsigned char *buffer = malloc(size);

    if (!buffer) {
        fprintf(stderr, "Error reading input file\n");
        fclose(file);
        return NULL;
    }

    const size_t bytes_read = fread(buffer, 1, size, file);
    fclose(file);

    if (bytes_read != size) {
        fprintf(stderr, "Error reading input file\n");
        free(buffer);
        return NULL;
    }

    *file_size = size;
    return buffer;
}
