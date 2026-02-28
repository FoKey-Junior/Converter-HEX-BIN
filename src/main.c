#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tools.h"

static int ends_with(const char *value, const char *suffix) {
    size_t value_len = strlen(value);
    size_t suffix_len = strlen(suffix);
    if (value_len < suffix_len) return 0;

    return strcmp(value + value_len - suffix_len, suffix) == 0;
}

static const char *find_ext(const char *path) {
    const char *slash1 = strrchr(path, '/');
    const char *slash2 = strrchr(path, '\\');
    const char *slash = slash1;
    if (slash2 && (!slash || slash2 > slash)) slash = slash2;

    const char *name = slash ? slash + 1 : path;
    const char *dot = strrchr(name, '.');
    if (!dot || dot == name) return NULL;

    return dot;
}

static char *make_output_name(const char *input, const char *new_ext) {
    const char *ext = find_ext(input);
    size_t base_len = ext ? (size_t)(ext - input) : strlen(input);
    size_t out_len = base_len + strlen(new_ext) + 1;

    char *out = malloc(out_len);
    if (!out) return NULL;

    memcpy(out, input, base_len);
    snprintf(out + base_len, out_len - base_len, "%s", new_ext);
    return out;
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 4) {
        fprintf(stderr, "The parameters have been entered incorrectly. If you do not know how to use the utility, use -h\n");
        return 1;
    }

    const char *input = argv[1];
    const char *opt = (argc >= 3) ? argv[2] : NULL;
    const char *output = (argc == 4) ? argv[3] : NULL;

    if (argc == 2 && strcmp(input, "-h") == 0) {
        help_documentation();
        return 0;
    }

    if (opt && strcmp(opt, "-h") == 0) {
        help_documentation();
        return 0;
    }

    const char *out_ext = NULL;
    int (*convert)(const char *, const char *) = NULL;

    if (opt) {
        if (strcmp(opt, "-a") == 0) {
            out_ext = ".bin";
            convert = conversion_to_bin;
        } else if (strcmp(opt, "-b") == 0) {
            out_ext = ".hex";
            convert = conversion_to_hex;
        } else {
            fprintf(stderr, "Unknown option: %s\n", opt);
            return 1;
        }
    } else {
        if (ends_with(input, ".hex")) {
            out_ext = ".bin";
            convert = conversion_to_bin;
        } else if (ends_with(input, ".bin")) {
            out_ext = ".hex";
            convert = conversion_to_hex;
        } else {
            fprintf(stderr, "Unsupported file extension for automatic conversion\n");
            return 1;
        }
    }

    if (output) {
        return convert(input, output);
    }

    char *auto_output = make_output_name(input, out_ext);

    if (!auto_output) {
        fprintf(stderr, "Memory allocation error\n");
        return 2;
    }

    int result = convert(input, auto_output);
    free(auto_output);
    return result;
}
