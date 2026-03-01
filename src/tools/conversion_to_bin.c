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

static int hex_value(unsigned char c) {
    if (c >= '0' && c <= '9') return (int)(c - '0');
    if (c >= 'a' && c <= 'f') return (int)(c - 'a' + 10);
    if (c >= 'A' && c <= 'F') return (int)(c - 'A' + 10);
    return -1;
}

static void write_bits(FILE *output, unsigned char byte) {
    for (int bit = 7; bit >= 0; bit--) {
        fputc(((byte >> bit) & 1) ? '1' : '0', output);
    }
}

static int write_bin_from_hex(const unsigned char *data, size_t size, FILE *output) {
    int have_high = 0;
    int high = 0;

    for (size_t i = 0; i < size; i++) {
        unsigned char c = data[i];
        if (isspace((int)c)) continue;

        int v = hex_value(c);
        if (v < 0) {
            fprintf(stderr, "Invalid HEX data\n");
            return 7;
        }

        if (!have_high) {
            high = v;
            have_high = 1;
        } else {
            unsigned char b = (unsigned char)((high << 4) | v);
            write_bits(output, b);
            have_high = 0;
        }
    }

    if (have_high) {
        fprintf(stderr, "Invalid HEX file: odd number of characters\n");
        return 5;
    }

    return 0;
}

static int write_bin_from_txt(const unsigned char *data, size_t size, FILE *output) {
    for (size_t i = 0; i < size; i++) {
        write_bits(output, data[i]);
    }
    return 0;
}

int conversion_to_bin(const char *input_path, const char *output_path) {
    size_t file_size = 0;
    unsigned char *file_contents = read_file(input_path, &file_size);
    if (!file_contents) return 3;

    FILE *output = fopen(output_path, "w");
    if (!output) {
        perror("Error opening output file");
        free(file_contents);
        return 6;
    }

    int rc = 0;
    if (ends_with(input_path, ".hex")) {
        rc = write_bin_from_hex(file_contents, file_size, output);
    } else if (ends_with(input_path, ".txt")) {
        rc = write_bin_from_txt(file_contents, file_size, output);
    } else {
        fprintf(stderr, "Unsupported file extension for BIN conversion\n");
        rc = 1;
    }

    fclose(output);
    free(file_contents);
    return rc;
}
