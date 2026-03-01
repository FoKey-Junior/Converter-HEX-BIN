#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/file_processing.h"
#include "../../include/tools.h"

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

static int write_txt_from_hex(const unsigned char *data, size_t size, FILE *out) {
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
            if (fwrite(&b, 1, 1, out) != 1) {
                fprintf(stderr, "Error writing output file\n");
                return 8;
            }
            have_high = 0;
        }
    }

    if (have_high) {
        fprintf(stderr, "Invalid HEX file: odd number of characters\n");
        return 5;
    }

    return 0;
}

static int write_txt_from_bin(const unsigned char *data, size_t size, FILE *out) {
    int bit_count = 0;
    unsigned char byte = 0;

    for (size_t i = 0; i < size; i++) {
        unsigned char c = data[i];
        if (isspace((int)c)) continue;

        if (c != '0' && c != '1') {
            fprintf(stderr, "Invalid BIN data\n");
            return 7;
        }

        byte = (unsigned char)((byte << 1) | (c == '1' ? 1 : 0));
        bit_count++;

        if (bit_count == 8) {
            if (fwrite(&byte, 1, 1, out) != 1) {
                fprintf(stderr, "Error writing output file\n");
                return 8;
            }
            bit_count = 0;
            byte = 0;
        }
    }

    if (bit_count != 0) {
        fprintf(stderr, "Invalid BIN data: number of bits is not a multiple of 8\n");
        return 5;
    }

    return 0;
}

int conversion_to_txt(const char *input_path, const char *output_path) {
    size_t file_size = 0;
    unsigned char *file_contents = read_file(input_path, &file_size);
    if (!file_contents) return 3;

    FILE *out = fopen(output_path, "wb");
    if (!out) {
        fprintf(stderr, "Error opening output file\n");
        free(file_contents);
        return 4;
    }

    int rc = 0;
    if (ends_with(input_path, ".hex")) {
        rc = write_txt_from_hex(file_contents, file_size, out);
    } else if (ends_with(input_path, ".bin")) {
        rc = write_txt_from_bin(file_contents, file_size, out);
    } else {
        fprintf(stderr, "Unsupported file extension for TXT conversion\n");
        rc = 1;
    }

    fclose(out);
    free(file_contents);
    return rc;
}
