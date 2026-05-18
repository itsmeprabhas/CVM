
# CVM++ : Stack-Based Virtual Machine & Custom Compiler

**CVM++** is a lightweight, custom scripting language built entirely from scratch in C++. This project demystifies the inner workings of compilers, interpreters, and runtime environments by breaking down the journey of raw text into executable bytecode.

Instead of relying on existing languages, CVM++ implements its own Lexical Analyzer, Recursive Descent Parser, AST generator, Bytecode Compiler, and a custom Stack-based Virtual Machine.


##  Pipeline Architecture

Ever wondered how Python or JavaScript executes your code? CVM++ replicates this pipeline at a microscopic level:

```text
Raw Source Code (.cvm)
       │
       ▼
  ┌─────────────┐
  │    LEXER    │  ──► Converts text into Tokens (e.g., NUMBER, PLUS, LET)
  └─────────────┘
       │
       ▼
  ┌─────────────┐
  │   PARSER    │  ──► Arranges Tokens into an Abstract Syntax Tree (AST)
  └─────────────┘
       │
       ▼
  ┌─────────────┐
  │  COMPILER   │  ──► Flattens AST into raw Bytecode (Opcodes & Operands)
  └─────────────┘
       │
       ▼
  ┌─────────────┐
  │      VM     │  ──► Executes Bytecode using a Stack and memory registers
  └─────────────┘
       │
       ▼
    Final Output
```

---

##  Language Features

- **Data Types:** Integers, Booleans (`true` / `false`)
- **Variables:** Declaration and assignment (`let x = 10;`, `x = x + 1;`)
- **Arithmetic:** `+`, `-`, `*`, `/`, `%`
- **Relational & Comparison:** `==`, `!=`, `<`, `>`, `<=`, `>=`
- **Bitwise:** `&`, `|`, `^`, `~`, `<<`, `>>`
- **Logical:** `and` / `&&`, `or` / `||`, `not` / `!`
- **Assignment Operators:** `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `&=`, `|=`, `^=`, `<<=`, `>>=`
- **Unary Update:** `++x`, `x++`, `--x`, `x--`
- **Control Flow:** `if / else`, `while`, `for`, `case/when`, `break`, `continue`
- **I/O:** `print`, `input`
- **Comments:** `// line comments`, `/* block comments */`

---

##  Tech Stack & Concepts

- **Language:** C++17
- **Build System:** CMake
- **Memory model:** `std::vector<uint8_t>` for bytecode, `std::vector<int32_t>` for VM stack and variables
- **Core concepts:**
  - Lexical analysis (tokenization)
  - Recursive descent parsing
  - AST + Visitor pattern
  - Bytecode generation
  - Stack-based virtual machine execution

---

##  Getting Started & Installation

### Prerequisites
- A C++17 compatible compiler (Apple Clang on macOS, GCC on Linux)
- CMake (optional, but recommended)

### Building the Project

Clone the repository and build it using CMake:

```bash
git clone https://github.com/itsmeprabhas/CVM.git
cd CVM
mkdir build && cd build
cmake ..
make
```

*(Alternatively, without CMake):*
```bash
clang++ -std=c++17 -Wall -I include src/*.cpp -o cvm++
```

---

##  Usage

### Run a script

```bash
./build/cvm++ tests/calculator.cvm
```

### Interactive REPL

```bash
./build/cvm++
```

### Debug mode

```bash
./build/cvm++ tests/calculator.cvm --debug
```

---

## Language Examples

### Variables, arithmetic, comparisons

```cvm
let a = 15;
let b = 5;
print a * b;
print a != b;
print a >= b;
```

### Control flow (`if`, `while`, `for`)

```cvm
let sum = 0;
for (let i = 1; i <= 5; i = i + 1) {
    sum += i;
}
if (sum > 10) {
    print sum;
}
```

### Bitwise and logical operators

```cvm
let x = 10;
let y = 3;
print x & y;
print x | y;
print x ^ y;
print x << 1;
print ~x;
print (x > y) and not (y == 0);
```

### Comments

```cvm
// line comment
let n = 4;
/* block
   comment */
print n;
```

### Case / When

```cvm
let score = 72;
case {
    when (score >= 90) { print 1; }
    when (score >= 75) { print 2; }
    when (score >= 60) { print 3; }
    else { print 4; }
}
```

---

## Project Structure

```text
CVM++/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── token.hpp
│   ├── lexer.hpp
│   ├── ast.hpp
│   ├── parser.hpp
│   ├── opcodes.hpp
│   ├── compiler.hpp
│   ├── vm.hpp
│   └── repl.hpp
├── src/
│   ├── token.cpp
│   ├── lexer.cpp
│   ├── ast.cpp
│   ├── parser.cpp
│   ├── opcodes.cpp
│   ├── compiler.cpp
│   ├── vm.cpp
│   ├── repl.cpp
│   └── main.cpp
└── tests/
    ├── test1.cvm
    ├── test2.cvm
    ├── test3.cvm
    ├── test4.cvm
    ├── test5.cvm
    └── test6.cvm
    |___ calculator.cvm 
```

---

## Instruction Set Architecture (ISA)

Representative opcodes:

- **Stack / data:** `PUSH`, `POP`, `DUP`, `LOAD`, `STORE`
- **Arithmetic:** `ADD`, `SUB`, `MUL`, `DIV`, `MOD`
- **Comparison:** `EQ`, `NEQ`, `LT`, `LE`, `GT`, `GE`
- **Bitwise / logical:** `BIT_AND`, `BIT_OR`, `BIT_XOR`, `SHL`, `SHR`, `AND`, `OR`, `NOT`
- **Control flow:** `JUMP`, `JUMP_IF_FALSE`, `JUMP_IF_TRUE`
- **I/O:** `PRINT`, `INPUT`
- **Program control:** `HALT`, `NOP`

---

## Acknowledgements

- "Writing a Lexer in C++"
- "Understanding Stack-Based Virtual Machines"
