#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../../include/tools.h"
#include "../../include/file_processing.h"

void conversion_to_hex(const char *input_directory, const char *output_name) {
    size_t input_size = 0;
    unsigned char *input_data = read_file(input_directory, &input_size);
    if (!input_data) exit(10);

    FILE *output_file = fopen(output_name, "w");

    if (!output_file) {
        fprintf(stderr, "Error opening output file\n");
        free(input_data);
        exit(11);
    }

    {
        const size_t input_path_len = strlen(input_directory);
        if (input_path_len >= 4 && strcmp(input_directory + input_path_len - 4, ".txt") == 0) {
        for (size_t index = 0; index < input_size; index++) {
            fprintf(output_file, "%02X", input_data[index]);
        }

        fclose(output_file);
        free(input_data);
        return;
        }
    }

    int bit_count = 0;
    unsigned char byte = 0;

    for (size_t index = 0; index < input_size; index++) {
        const unsigned char input_char = input_data[index];
        if (isspace((int)input_char)) {
            continue;
        }

        if (input_char != '0' && input_char != '1') {
            fprintf(stderr, "Invalid BIN data\n");
            fclose(output_file);
            free(input_data);
            exit(32);
        }

        byte = (unsigned char)((byte << 1) | (input_char == '1' ? 1 : 0));
        bit_count++;

        if (bit_count == 8) {
            fprintf(output_file, "%02X", byte);
            bit_count = 0;
            byte = 0;
        }
    }

    if (bit_count != 0) {
        fprintf(stderr, "Invalid BIN data: number of bits is not a multiple of 8\n");
        fclose(output_file);
        free(input_data);
        exit(33);
    }

    fclose(output_file);
    free(input_data);
}
