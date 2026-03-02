#include <stdio.h>

#include "../../include/tools.h"

void help_documentation() {
    printf(
        "\033[0d\033[2J"
        "ASCII HEX / Binary Converter Utility\n\n"
        "Description:\n"
        "  The utility converts files between ASCII HEX and ASCII binary (0/1) formats\n"
        "  The input file name is specified first. The option specifies the conversion direction\n"
        "  The output file name can be provided explicitly or generated automatically based on the input file name:\n"
        "    HEX -> BIN: extension is replaced with .bin\n"
        "    BIN -> HEX: extension is replaced with .hex\n"
        "    HEX/BIN -> TXT: extension is replaced with .txt\n\n"
        "Syntax:\n"
        "  <input_file_name> <option> [output_file_name]\n\n"
        "Options:\n"
        "  -t    Convert to TXT (.txt)\n"
        "  -h    Convert to HEX (.hex)\n"
        "  -b    Convert to BIN (.bin)\n\n"
        "  -h    Convert the input BIN (.bin) or TXT (.txt) file to a HEX file\n\n"
        "Help:\n"
        "  -h    Display this help message\n\n"
        "Notes:\n"
        "  The input file must exist and be readable\n"
        "  BIN format in this project is a text file containing only 0/1 characters (whitespace is ignored)\n"
        "  TXT conversion decodes HEX or BIN into raw bytes and writes them to a .txt file\n"
        "  If an unknown option or unsupported extension is passed\n"
        "  the utility terminates with an error message\n"
    );
}
