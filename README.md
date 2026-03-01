<div align="center">

## ASCII HEX / Binary Converter (C)

[![C](https://img.shields.io/badge/C-11-blue.svg)](https://en.cppreference.com/w/c)
[![CTest](https://img.shields.io/badge/CTest-blue.svg)](https://cmake.org/cmake/help/latest/manual/ctest.1.html)
[![CMake](https://img.shields.io/badge/CMake-3.28%2B-orange.svg)](https://cmake.org/)

Небольшая консольная утилита на чистом C для конвертации файлов между форматами ASCII HEX и Бинарным кодом (0/1) (HEX ↔ BIN)

---

### Инструкция по использование

1: открыть инструкцию

```bash
./converter -h
```

2: ручная конвертация (явное направление)

```bash
./converter text.hex -a text.bin
./converter text.bin -b text.hex
```

3: конвертация в `.txt` через `-t`

```bash
./converter text.hex -t text.txt
./converter text.bin -t text.txt
```

---

### Тесты

```bash
ctest --test-dir build --output-on-failure
```

### Ресурсы, использованные при разработке

В качестве справочных материалов во время разработки использовались:

https://github.com/FoKey-Junior/Blockchain

https://cmake.org/cmake/help/latest/manual/ctest.1.html
</div>
