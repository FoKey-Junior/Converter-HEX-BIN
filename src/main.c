#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tools.h"

int main(int argc, char *argv[]) {

    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        help_documentation();
        return 0;
    }

    if (argc < 3 || argc > 4) {
        fprintf(stderr,
                "Incorrect parameters. Use -h for help\n");
        return 2;
    }

    const char *input_directory = argv[1];
    const char *option = argv[2];

    const char *extension = NULL;
    void (*convert)(const char *, const char *) = NULL;

    if (option[0] != '-' || option[2] != '\0') {
        fprintf(stderr, "Incorrect option: %s\n", option);
        return 2;
    }

    switch (option[1]) {
        case 'b':
            extension = ".bin";
            convert = conversion_to_bin;
            break;

        case 't':
            extension = ".txt";
            convert = conversion_to_txt;
            break;

        case 'h':
            extension = ".hex";
            convert = conversion_to_hex;
            break;

        default:
            fprintf(stderr, "Unknown option: %s\n", option);
            return 3;
    }

    char *output_directory = NULL;

    if (argc == 4) {
        output_directory = argv[3];
    } else {

        const char *dot = strrchr(input_directory, '.');
        size_t base_len = dot ? (size_t)(dot - input_directory)
                              : strlen(input_directory);

        size_t len = base_len + strlen(extension) + 1;

        output_directory = malloc(len);
        if (!output_directory) {
            fprintf(stderr, "Memory allocation failed\n");
            return 4;
        }

        snprintf(output_directory, len, "%.*s%s",
                 (int)base_len,
                 input_directory,
                 extension);
    }

    convert(input_directory, output_directory);

    if (argc != 4)
        free(output_directory);

    return 0;
}