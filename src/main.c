#include <stdio.h>
#include <stdlib.h>

#include "../include/tools.h"

int main(int argc, char *argv[]) {
    if (argv[1][1] == 'h') {
        help_documentation();
        return 1;
    }

    if (argc == 4) {
        fprintf(stderr, "The parameters have been entered incorrectly. If you do not know how to use the utility, use -h\n");
        return 2;
    }

    const char *input = argv[1];
    const char option = argv[2][1];
    const char *output = argv[3];

    switch (option) {
        case 'a':
            conversion_to_bin(input, output);
            break;
        case 'b':
            conversion_to_hex(input, output);
            break;
        default:
            if (option != '\0') {
                fprintf(stderr, "Unknown option: -%c\n", option);
            }
            break;
    }

    return 0;
}
