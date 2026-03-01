#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../../include/tools.h"
#include "../../include/file_processing.h"

static int ends_with(const char *value, const char *suffix) {
    size_t value_len = strlen(value);
    size_t suffix_len = strlen(suffix);
    if (value_len < suffix_len) return 0;
    return strcmp(value + value_len - suffix_len, suffix) == 0;
}

int conversion_to_hex(const char *input_directory, const char *output_name) {
    size_t file_size = 0;
    unsigned char *file_contents = read_file(input_directory, &file_size);
    if (!file_contents) return 3;

    FILE *output = fopen(output_name, "w");

    if (!output) {
        fprintf(stderr, "Error opening output file\n");
        free(file_contents);
        return 4;
    }

    if (ends_with(input_directory, ".txt")) {
        for (size_t i = 0; i < file_size; i++) {
            fprintf(output, "%02X", file_contents[i]);
        }

        fclose(output);
        free(file_contents);
        return 0;
    }

    int bit_count = 0;
    unsigned char byte = 0;

    for (size_t i = 0; i < file_size; i++) {
        unsigned char c = file_contents[i];
        if (isspace((int)c)) {
            continue;
        }

        if (c != '0' && c != '1') {
            fprintf(stderr, "Invalid BIN data\n");
            fclose(output);
            free(file_contents);
            return 7;
        }

        byte = (unsigned char)((byte << 1) | (c == '1' ? 1 : 0));
        bit_count++;

        if (bit_count == 8) {
            fprintf(output, "%02X", byte);
            bit_count = 0;
            byte = 0;
        }
    }

    if (bit_count != 0) {
        fprintf(stderr, "Invalid BIN data: number of bits is not a multiple of 8\n");
        fclose(output);
        free(file_contents);
        return 5;
    }

    fclose(output);
    free(file_contents);
    return 0;
}
