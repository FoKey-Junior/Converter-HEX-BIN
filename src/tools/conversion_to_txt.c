#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/file_processing.h"
#include "../../include/tools.h"

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

static void write_txt_from_hex(const unsigned char *hex_text, size_t hex_text_size, FILE *output_file) {
    int has_high_nibble = 0;
    int high_nibble = 0;

    for (size_t index = 0; index < hex_text_size; index++) {
        const unsigned char input_char = hex_text[index];
        if (isspace((int)input_char)) continue;

        const int nibble = hex_value(input_char);
        if (nibble < 0) {
            fprintf(stderr, "Invalid HEX data\n");
            exit(30);
        }

        if (!has_high_nibble) {
            high_nibble = nibble;
            has_high_nibble = 1;
        } else {
            const unsigned char byte = (unsigned char)((high_nibble << 4) | nibble);
            if (fwrite(&byte, 1, 1, output_file) != 1) {
                fprintf(stderr, "Error writing output file\n");
                exit(12);
            }
            has_high_nibble = 0;
        }
    }

    if (has_high_nibble) {
        fprintf(stderr, "Invalid HEX file: odd number of characters\n");
        exit(31);
    }
}

static void write_txt_from_bin(const unsigned char *bit_text, size_t bit_text_size, FILE *output_file) {
    int bit_count = 0;
    unsigned char byte = 0;

    for (size_t index = 0; index < bit_text_size; index++) {
        const unsigned char input_char = bit_text[index];
        if (isspace((int)input_char)) continue;

        if (input_char != '0' && input_char != '1') {
            fprintf(stderr, "Invalid BIN data\n");
            exit(32);
        }

        byte = (unsigned char)((byte << 1) | (input_char == '1' ? 1 : 0));
        bit_count++;

        if (bit_count == 8) {
            if (fwrite(&byte, 1, 1, output_file) != 1) {
                fprintf(stderr, "Error writing output file\n");
                exit(12);
            }
            bit_count = 0;
            byte = 0;
        }
    }

    if (bit_count != 0) {
        fprintf(stderr, "Invalid BIN data: number of bits is not a multiple of 8\n");
        exit(33);
    }
}

void conversion_to_txt(const char *input_path, const char *output_path) {
    size_t input_size = 0;
    unsigned char *input_data = read_file(input_path, &input_size);
    if (!input_data) exit(10);

    FILE *output_file = fopen(output_path, "wb");
    if (!output_file) {
        fprintf(stderr, "Error opening output file\n");
        free(input_data);
        exit(11);
    }

    if (ends_with(input_path, ".hex")) {
        write_txt_from_hex(input_data, input_size, output_file);
    } else if (ends_with(input_path, ".bin")) {
        write_txt_from_bin(input_data, input_size, output_file);
    } else {
        fprintf(stderr, "Unsupported file extension for TXT conversion\n");
        fclose(output_file);
        free(input_data);
        exit(20);
    }

    fclose(output_file);
    free(input_data);
}
