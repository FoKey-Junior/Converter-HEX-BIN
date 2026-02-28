#include <stdio.h>

#include "../../include/tools.h"

void help_documentation() {
    printf(
        "\033[0d\033[2J"
        "ASCII HEX / Binary Converter Utility\n\n"
        "Description:\n"
        "  The utility converts files between ASCII HEX and ASCII binary (0/1) formats\n"
        "  The input file name is specified first. The option specifies the conversion direction\n"
        "  The output file name is generated automatically based on the input file name with the appropriate extension added:\n"
        "    HEX -> BIN: .bin is added\n"
        "    BIN -> HEX: .hex is added\n\n"
        "Syntax:\n"
        "  <input_file_name> [option] [output_file_name]\n\n"
        "Options:\n"
        "  -a    Convert the input HEX file to a BIN file (0/1 text)\n"
        "        Example:\n"
        "          text.hex -a  text.bin\n\n"
        "  -b    Convert the input BIN file (0/1 text) to a HEX file\n"
        "        Example:\n"
        "          text.bin -b  text.hex\n\n"
        "  -h    Display this help message\n\n"
        "Behavior without options:\n"
        "  If only the input file name is specified, the utility automatically\n"
        "  performs the opposite conversion based on the file extension:\n"
        "    .hex -> creates a binary file with the extension .bin\n"
        "    .bin -> creates a HEX file with the extension .hex\n"
        "  Examples:\n"
        "    text.hex  text.bin\n"
        "    text.bin  text.hex\n\n"
        "Notes:\n"
        "  The input file must exist and be readable\n"
        "  BIN format in this project is a text file containing only 0/1 characters (whitespace is ignored)\n"
        "  If an unknown option or unsupported extension is passed\n"
        "  the utility terminates with an error message\n"
    );
}
