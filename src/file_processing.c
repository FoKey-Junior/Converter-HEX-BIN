#include <stdio.h>
#include <stdlib.h>

#include "../include/file_processing.h"

unsigned char* read_file(const char *file_directory, size_t *file_size) {
    *file_size = 0;
    FILE *input = fopen(file_directory, "rb");

    if (!input) {
        fprintf(stderr, "Error opening input file\n");
        return NULL;
    }

    fseek(input, 0, SEEK_END);
    size_t size = ftell(input);
    fseek(input, 0, SEEK_SET);

    if (size == 0) {
        fprintf(stderr, "Input file is empty\n");
        fclose(input);
        return NULL;
    }

    unsigned char *buffer = malloc(size);

    if (!buffer) {
        fprintf(stderr, "Error reading input file\n");
        fclose(input);
        return NULL;
    }

    const size_t bytes_read = fread(buffer, 1, size, input);
    fclose(input);

    if (bytes_read != size) {
        fprintf(stderr, "Error reading input file\n");
        free(buffer);
        return NULL;
    }

    *file_size = size;
    return buffer;
}
