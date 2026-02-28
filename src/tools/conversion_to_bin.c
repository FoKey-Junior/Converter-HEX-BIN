#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../../include/tools.h"
#include "../../include/file_processing.h"

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

    int have_high = 0;
    int high = 0;

    for (size_t i = 0; i < file_size; i++) {
        unsigned char c = file_contents[i];
        if (isspace((int)c)) {
            continue;
        }

        int v = hex_value(c);
        if (v < 0) {
            fprintf(stderr, "Invalid HEX data\n");
            fclose(output);
            free(file_contents);
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
        fclose(output);
        free(file_contents);
        return 5;
    }

    fclose(output);
    free(file_contents);
    return 0;
}
