#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tools.h"

int main(const int argc, char *argv[]) {
    if (argc > 4) {
        fprintf(stderr, "The parameters have been entered incorrectly. If you do not know how to use the utility, use -h\n");
        return 1;
    }

    if (strcmp(argv[1], "-h") == 0) {
        help_documentation();
        return 0;
    }

    const char *input_directory = argv[1];
    const char *option = argv[2];
    char *output_directory;
    const char *option_extension;
    int (*convert)(const char *, const char *);

    if (option[0] != '-') {
        fprintf(stderr, "Incorrectly entered option: %s\n", option);
        return 2;
    }

    switch (option[1]) {
        case 'b': option_extension = ".bin"; convert = conversion_to_bin; break;
        case 'h': option_extension = ".hex"; convert = conversion_to_hex; break;
        case 't': option_extension = ".txt"; convert = conversion_to_txt; break;
        default:
            fprintf(stderr, "Unknown variant: -%c\n", option[1]);
            return 3;
    }

    if (argc == 4) {
        output_directory = argv[3];
        convert(input_directory, output_directory);
    } else {
        const size_t output_length = strlen(input_directory) + strlen(option_extension) + 1;
        output_directory = malloc(output_length);

        if (!output_directory) {
            fprintf(stderr, "Memory allocation failed\n");
            return 4;
        }

        strcpy(output_directory, input_directory);
        char *expansion_position = strrchr(output_directory, '.');
        if (expansion_position) *expansion_position = '\0';
        strcat(output_directory, option_extension);

        convert(input_directory, output_directory);
        free(output_directory);
    }

    return 0;
}