#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path, mode) _mkdir(path)
#define PATH_SEP '\\'
#else
#define MKDIR(path, mode) mkdir(path, mode)
#define PATH_SEP '/'
#endif

#include "../include/file_processing.h"
#include "../include/tools.h"

enum { PATH_CAP = 256 };

static const char *g_test_dir = "ut_tmp";

static void die_create_dir(void) {
    fprintf(stderr, "failed to create test dir\n");
    exit(1);
}

static void check(int ok, const char *expr, int line) {
    if (!ok) {
        fprintf(stderr, "check failed at line %d: %s\n", line, expr);
        exit(1);
    }
}

#define CHECK(x) check((x) != 0, #x, __LINE__)

static void path_in_dir(char *out, const char *name) {
    int written = snprintf(out, PATH_CAP, "%s%c%s", g_test_dir, PATH_SEP, name);
    CHECK(written > 0);
    CHECK(written < PATH_CAP);
}

static void ensure_test_dir(void) {
    if (MKDIR(g_test_dir, 0700) == 0) {
        return;
    }

    if (errno == EEXIST) {
        return;
    }

    die_create_dir();
}

static void write_file(const char *path, const unsigned char *data, size_t size) {
    FILE *f = fopen(path, "wb");
    CHECK(f != NULL);

    if (size) {
        CHECK(fwrite(data, 1, size, f) == size);
    }

    CHECK(fclose(f) == 0);
}

static size_t read_file_small(const char *path, unsigned char *buf, size_t cap, const char *mode) {
    FILE *f = fopen(path, mode);
    CHECK(f != NULL);
    size_t n = fread(buf, 1, cap, f);
    CHECK(fclose(f) == 0);

    return n;
}

static void test_basic(void) {
    char in_hex[PATH_CAP] = {0};
    char out_bin[PATH_CAP] = {0};
    char out_hex[PATH_CAP] = {0};
    char out_txt[PATH_CAP] = {0};
    path_in_dir(in_hex, "basic_input.hex");
    path_in_dir(out_bin, "basic_output.bin");
    path_in_dir(out_hex, "basic_output.hex");
    path_in_dir(out_txt, "basic_output.txt");
    remove(in_hex);
    remove(out_bin);
    remove(out_hex);
    remove(out_txt);

    {
        const unsigned char hex_text[] = "48 65 6C\n6C 6F";
        write_file(in_hex, hex_text, strlen((const char *)hex_text));
    }

    CHECK(conversion_to_bin(in_hex, out_bin) == 0);

    {
        char bits[64] = {0};
        size_t n = read_file_small(out_bin, (unsigned char *)bits, sizeof(bits) - 1, "r");
        CHECK(n == 40);
        CHECK(strcmp(bits, "0100100001100101011011000110110001101111") == 0);
    }

    CHECK(conversion_to_hex(out_bin, out_hex) == 0);

    {
        char text[32] = {0};
        size_t n = read_file_small(out_hex, (unsigned char *)text, sizeof(text) - 1, "r");
        CHECK(n == 10);
        CHECK(strcmp(text, "48656C6C6F") == 0);
    }

    CHECK(conversion_to_txt(in_hex, out_txt) == 0);
    {
        char text[32] = {0};
        size_t n = read_file_small(out_txt, (unsigned char *)text, sizeof(text) - 1, "rb");
        CHECK(n == 5);
        CHECK(memcmp(text, "Hello", 5) == 0);
    }
}

int main(void) {
    ensure_test_dir();
    test_basic();

    return 0;
}
