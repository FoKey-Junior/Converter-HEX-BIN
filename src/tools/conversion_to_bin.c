#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../../include/tools.h"
#include "../../include/file_processing.h"

static int ends_with(const char *text, const char *suffix) {
    const size_t text_len = strlen(text);
    const size_t suffix_len = strlen(suffix);
    if (text_len < suffix_len) return 0;
    return strcmp(text + text_len - suffix_len, suffix) == 0;
}

static int hex_value(unsigned char hex_char) {
    if (hex_char >= '0' && hex_char <= '9') return (int)(hex_char - '0');
    if (hex_char >= 'a' && hex_char <= 'f') return (int)(hex_char - 'a' + 10);
    if (hex_char >= 'A' && hex_char <= 'F') return (int)(hex_char - 'A' + 10);
    return -1;
}

static void write_bits(FILE *output, unsigned char byte) {
    for (int bit_index = 7; bit_index >= 0; bit_index--) {
        fputc(((byte >> bit_index) & 1) ? '1' : '0', output);
    }
}

static void write_bin_from_hex(const unsigned char *hex_text, size_t hex_text_size, FILE *output) {
    int has_high_nibble = 0;
    int high_nibble = 0;

    for (size_t index = 0; index < hex_text_size; index++) {
        const unsigned char ch = hex_text[index];
        if (isspace((int)ch)) continue;

        const int nibble = hex_value(ch);
        if (nibble < 0) {
            fprintf(stderr, "Invalid HEX data\n");
            exit(30);
        }

        if (!has_high_nibble) {
            high_nibble = nibble;
            has_high_nibble = 1;
        } else {
            const unsigned char byte = (unsigned char)((high_nibble << 4) | nibble);
            write_bits(output, byte);
            has_high_nibble = 0;
        }
    }

    if (has_high_nibble) {
        fprintf(stderr, "Invalid HEX file: odd number of characters\n");
        exit(31);
    }
}

static void write_bin_from_txt(const unsigned char *bytes, size_t bytes_count, FILE *output) {
    for (size_t index = 0; index < bytes_count; index++) {
        write_bits(output, bytes[index]);
    }
}

void conversion_to_bin(const char *input_path, const char *output_path) {
    size_t input_size = 0;
    unsigned char *input_data = read_file(input_path, &input_size);
    if (!input_data) exit(10);

    FILE *output_file = fopen(output_path, "w");
    if (!output_file) {
        perror("Error opening output file");
        free(input_data);
        exit(11);
    }

    if (ends_with(input_path, ".hex")) {
        write_bin_from_hex(input_data, input_size, output_file);
    } else if (ends_with(input_path, ".txt")) {
        write_bin_from_txt(input_data, input_size, output_file);
    } else {
        fprintf(stderr, "Unsupported file extension for BIN conversion\n");
        fclose(output_file);
        free(input_data);
        exit(20);
    }

    fclose(output_file);
    free(input_data);
}
