#include <stdio.h>
#include <stdlib.h>

#include "../../include/tools.h"
#include "../../include/file_processing.h"

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

    for (size_t i = 0; i < file_size; i++) {
        fprintf(output, "%02X", file_contents[i]);
    }

    fclose(output);
    free(file_contents);
    return 0;
}
