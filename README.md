<div align="center">

## ASCII HEX / Binary Converter (C)

[![C](https://img.shields.io/badge/C-11-blue.svg)](https://en.cppreference.com/w/c)
[![CTest](https://img.shields.io/badge/CTest-4.1.2-2E9F5B.svg)](https://cmake.org/cmake/help/latest/manual/ctest.1.html)
[![CMake](https://img.shields.io/badge/CMake-3.16%2B-orange.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A small, pure C console utility for converting files between ASCII HEX and binary code (0/1) formats (HEX ↔ BIN)

---

### Usage Instructions

1: open instructions

```bash
./converter -h
```

---

Options:

`-t` - convert to `.txt`

`-h` - convert to `.hex`

`-b` - convert to `.bin`

---

2: manual conversion (explicit direction)

```bash
./converter text.bin -h text.hex
./converter text.hex -b text.bin
```

2.1: conversion without specifying an output file (name is generated automatically)

```bash
./converter text.bin -h
./converter text.hex -b
./converter text.hex -t
```

3: conversion to `.txt` using `-t`

```bash
./converter text.hex -t text.txt
./converter text.bin -t text.txt
```

---

### Tests

```bash
ctest --test-dir build --output-on-failure
```
</div>
