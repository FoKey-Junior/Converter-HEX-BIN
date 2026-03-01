#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tools.h"

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 4) {
        fprintf(stderr, "The parameters have been entered incorrectly. If you do not know how to use the utility, use -h\n");
        return 1;
    }

    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        help_documentation();
        return 0;
    }

    const char *input = argv[1];
    const char *opt = argc >= 3 ? argv[2] : NULL;
    const char *explicit_output = argc == 4 ? argv[3] : NULL;

    if (opt && strcmp(opt, "-h") == 0) {
        help_documentation();
        return 0;
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
        size_t input_len = strlen(input);
        if (input_len >= 4 && strcmp(input + input_len - 4, ".hex") == 0) {
            out_ext = ".bin"; convert = conversion_to_bin;
        } else if (input_len >= 4 && strcmp(input + input_len - 4, ".bin") == 0) {
            out_ext = ".hex"; convert = conversion_to_hex;
        } else {
            fprintf(stderr, "Unsupported file extension for automatic conversion\n");
            return 1;
        }
    }

    if (explicit_output) {
        return convert(input, explicit_output);
    }

    const char *slash1 = strrchr(input, '/');
    const char *slash2 = strrchr(input, '\\');
    const char *slash = slash1;
    if (slash2 && (!slash || slash2 > slash)) {
        slash = slash2;
    }

    const char *name = slash ? slash + 1 : input;
    const char *dot = strrchr(name, '.');
    const char *ext = (!dot || dot == name) ? NULL : dot;

    size_t base_len = ext ? (size_t)(ext - input) : strlen(input);
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
