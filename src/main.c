#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tools.h"

int main(const int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        help_documentation();
        return 0;
    }

    if (argc < 3 || argc > 4) {
        fprintf(stderr, "The parameters have been entered incorrectly. If you do not know how to use the utility, use -h\n");
        return 2;
    }

    const char *const input_directory = argv[1];
    const char *const option = argv[2];
    char *output_directory;
    const char *option_extension;
    void (*convert)(const char *, const char *);

    if (option[0] != '-') {
        fprintf(stderr, "Incorrectly entered option: %s\n", option);
        return 2;
    }

    switch (option[1]) {
        case 'b': option_extension = ".bin"; convert = conversion_to_bin; break;
        case 't': option_extension = ".txt"; convert = conversion_to_txt; break;
        case 'h': option_extension = ".hex"; convert = conversion_to_hex; break;
        default:
            fprintf(stderr, "Unknown variant: -%c\n", option[1]);
            return 3;
    }

    if (argc == 4) {
        output_directory = argv[3];
        convert(input_directory, output_directory);
    } else {
        size_t len = strlen(input_directory) + strlen(option_extension) + 1;
        output_directory = malloc(len);
        if (!output_directory) {
            fprintf(stderr, "Memory allocation failed\n");
            return 4;
        }

        char *dot = strrchr(input_directory, '.');
        if (dot) {
            size_t base_len = dot - input_directory;
            memcpy(output_directory, input_directory, base_len);
            output_directory[base_len] = '\0';
        } else {
            strcpy(output_directory, input_directory);
        }
        strcat(output_directory, option_extension);

        convert(input_directory, output_directory);
        free(output_directory);
    }

    return 0;
}