cmake_minimum_required(VERSION 3.28)
project(test_assignment C)

set(CMAKE_C_STANDARD 11)
enable_testing()

add_executable(converter
        src/main.c
        src/file_processing.c
        src/tools/conversion_to_bin.c
        src/tools/conversion_to_hex.c
        src/tools/documentation.c
)

add_executable(unit_tests
        tests/unit_tests.c
        src/file_processing.c
        src/tools/conversion_to_bin.c
        src/tools/conversion_to_hex.c
)

add_test(NAME unit_tests COMMAND unit_tests)
