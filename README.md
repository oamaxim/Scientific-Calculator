# Scientific Calculator (C++ / Qt)

A scientific calculator built in C++ using Qt.  
It supports expressions, functions, matrices, variables, and constants, with a custom-built lexer, parser, and evaluator.

This project demonstrates compiler-style design (tokenisation → parsing → AST → evaluation), GUI development with Qt, and advanced expression handling.

---

## Features

- Basic arithmetic: `+ - * / ^`
- Scientific functions:
  - Trigonometry: `sin`, `cos`, `tan`, `cosec`, `sec`, `cot`
  - Inverse trig: `asin`, `acos`, `atan`
  - Hyperbolic functions: `sinh`, `cosh`, `tanh`
- Logarithmic functions: `log`, `log10`, `ln`
- Constants:
  - `pi`, `e`, `tau`, `phi`, `ans`
- Variables:
  - Assign and reuse values (e.g. `x = 5`)
- Matrices:
  - Creation: `mat(rows, cols, values...)`
  - Operations: `+ - * / ^`
  - Functions: `det()`, `transpose()`, `inv()`, `identity()`
- Implicit multiplication (e.g. `2x`, `2(3+1)`)
- Angle mode toggle: Degrees / Radians
- History system with evaluation log
- SHIFT mode for alternate function mappings
- GUI built with Qt (custom buttons, layout system)

---

## Example Usage

2 + 3 * 4

sin(90)

x = 5

x^2

mat(2,2,1,2,3,4)

det(mat(2,2,1,2,3,4))

ans + 10

---

## Tech Stack

- C++17
- Qt (Widgets)
- Custom Lexer / Parser / AST system
- CMake build system

---

## Architecture

The calculator is built in 4 main layers:

1. **Lexer**
   - Converts input string into tokens

2. **Parser**
   - Builds an Abstract Syntax Tree (AST)

3. **Evaluator**
   - Recursively evaluates AST nodes

4. **GUI (Qt)** or **CLI**
   - Handles input, buttons, and history display

---

## How to Build

### Prerequisites
- CMake
- Qt 6 (or Qt 5 with MinGW)
- C++ compiler (GCC / MSVC)

### Build Steps

```bash
git clone https://github.com/maxim-oa/Scientific-Calculator.git
cd Scientific-Calculator
mkdir build
cd build
cmake ..
cmake --build .
```

### Run

```bash
./gui.exe
```

**or**

```bash
./cli.exe
```

## Author

Built by OMA as a personal project to explore compilers, math engines, and GUI design in C++.