#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tools.h"

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 4) {
        fprintf(stderr, "The parameters have been entered incorrectly. If you do not know how to use the utility, use -h\n");
        return 1;
    }

    const char *input = argv[1];
    const char *opt = argc >= 3 ? argv[2] : NULL;

    if ((argc == 2 && strcmp(input, "-h") == 0) || (opt && strcmp(opt, "-h") == 0)) {
        help_documentation();
        return 0;
    }

    if (argc == 2) {
        fprintf(stderr, "The parameters have been entered incorrectly. If you do not know how to use the utility, use -h\n");
        return 1;
    }

    const char *out_ext = NULL;
    int (*convert)(const char *, const char *) = NULL;

    if (opt) {
        if (opt[0] != '-' || opt[1] == '\0' || opt[2] != '\0') {
            fprintf(stderr, "Unknown option: %s\n", opt);
            return 1;
        }

        switch (opt[1]) {
            case 'a': out_ext = ".bin"; convert = conversion_to_bin; break;
            case 'b': out_ext = ".hex"; convert = conversion_to_hex; break;
            case 't': out_ext = ".txt"; convert = conversion_to_txt; break;
            default:
                fprintf(stderr, "Unknown option: -%c\n", opt[1]);
                return 1;
        }
    } else {
        fprintf(stderr, "The parameters have been entered incorrectly. If you do not know how to use the utility, use -h\n");
        return 1;
    }

    if (argc == 4) {
        return convert(input, argv[3]);
    }

    size_t input_len = strlen(input);
    const char *dot = NULL;
    for (size_t i = input_len; i > 0; i--) {
        char c = input[i - 1];
        if (c == '.') {
            dot = input + i - 1;
            break;
        }
        if (c == '/' || c == '\\') {
            break;
        }
    }

    size_t base_len = dot ? (size_t)(dot - input) : input_len;
    size_t out_len = base_len + strlen(out_ext) + 1;
    char *auto_output = malloc(out_len);
    if (!auto_output) {
        fprintf(stderr, "Memory allocation error\n");
        return 2;
    }

    memcpy(auto_output, input, base_len);
    snprintf(auto_output + base_len, out_len - base_len, "%s", out_ext);

    int rc = convert(input, auto_output);
    free(auto_output);
    return rc;
}
