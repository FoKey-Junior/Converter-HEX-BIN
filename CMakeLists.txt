cmake_minimum_required(VERSION 3.16)
project(test_assignment C)

set(CMAKE_C_STANDARD 11)
enable_testing()

add_library(core
        src/file_processing.c
        src/tools/conversion_to_bin.c
        src/tools/conversion_to_hex.c
        src/tools/conversion_to_txt.c
        src/tools/help_documentation.c
)

add_executable(converter
        src/main.c
)

target_link_libraries(converter core)

add_executable(unit_tests
        tests/unit_tests.c
)

target_link_libraries(unit_tests core)

add_test(NAME unit_tests COMMAND unit_tests)
