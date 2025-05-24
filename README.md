# VizMach: Virtual Assembly Machine Simulator in C

This repository implements **VizMach**, a virtual machine (VM) in C that simulates a basic assembly-like language with 30 instructions. The project is designed as a university assignment (CSE102 Assignment 10) to help students understand CPU architecture, instruction sets, and program execution at a low level.

---

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Instruction Set](#instruction-set)
- [File Formats](#file-formats)
- [How to Compile & Run](#how-to-compile--run)
- [Usage](#usage)
- [Sample Input & Output](#sample-input--output)
- [Implementation Notes](#implementation-notes)
- [Author](#author)

---

## Overview

**VizMach** is a C-based emulator for a simple computer architecture. It loads assembly code from `.asm` files, parses and executes instructions based on a custom instruction set defined in `instructions 1.txt`. The simulator mimics basic CPU components such as registers, program counter, instruction register, and main memory.

---

## Features

- Two 8-bit general purpose registers: `A` and `B`
- 16-bit Program Counter (PC)
- 24-bit Instruction Register (IR)
- 108-byte memory array (`memory[108][9]`)
- Loads 30 custom instructions from an external file
- Supports arithmetic, logic, data movement, branching, I/O, and program control
- Executes any valid `.asm` file written in the provided assembly language
- Terminal menu: allows repeated execution with different code files
- Output displays instruction results and program output

---

## System Architecture

- **Registers:**
    - `A`, `B`: 8 bits each
    - `PC`: 16 bits, holds instruction address
    - `IR`: 24 bits, holds current instruction
- **Memory:**
    - `memory[108][9]`: Array of 108 bytes, each byte as 8-character string (plus null terminator)
- **Instruction Set:**
    - Loaded from `instructions 1.txt`
    - Each instruction includes opcode, mnemonic, and explanation

---

## Instruction Set

Instructions are loaded from `instructions 1.txt` and include arithmetic, logical, memory, control, and I/O operations.  
Example (full list in the file):

| Opcode    | Mnemonic         | Description                          |
|-----------|------------------|--------------------------------------|
| 00000001  | ADD A B          | addition operation                   |
| 00000010  | ADDI A I         | addition with immediate number       |
| 00001010  | SUB A B          | subtraction operation                |
| 00010001  | BRE A B          | branch if A == B                     |
| 00011010  | PRI A            | print register A                     |
| ...       | ...              | ...                                  |

> See `instructions 1.txt` for the full 30-instruction set with opcodes, formats, and explanations:contentReference[oaicite:0]{index=0}.

---

## File Formats

- **`instructions 1.txt`**  
  Contains 30 instructions, each with a unique opcode, type, and explanation.  
  Each line format:
  
OPCODE|TYPE|MNEMONIC|DESCRIPTION
- **Assembly Source File (`.asm`)**  
Each line: one instruction and its operands (e.g., `LDAI A 100`).  
Example:
LDAI A 100
LDAI B 50
ADD A B
PRI A
EXIT

- **C Source (`220104004043.c`)**  
Main project source code (your VizMach VM).

---

## How to Compile & Run

1. **Ensure you have all files in the same directory:**
  - `220104004043.c`
  - `instructions 1.txt`
  - At least one `.asm` file (your assembly program)
2. **Compile the project (ANSI C):**
  ```bash
  gcc --ansi 220104004043.c -o vizmach
  ```
3. **Run the simulator:**
  ```bash
  ./vizmach
  ```
4. **When prompted, enter the filename of your `.asm` code to execute.**

---

## Usage

- Enter the filename of your assembly program (with `.asm` extension) when prompted.
- Program will load and execute all instructions, outputting results to the terminal.
- After each run, you can execute another program or exit.
- All instruction logic (arithmetic, logic, memory, branching, print, halt) are supported as per `instructions 1.txt`.

---

## Sample Input & Output

### **Example assembly file (`sum.asm`):**
LDAI A 40
LDAI B 15
ADD A B
PRI A
EXIT


### **Terminal Output:**
Enter the filename: sum.asm
Result of execution: 55
Do you want to execute another assembly code: Press(1) for yes Press(0) for exit: 0


---

## Implementation Notes

- Uses a custom struct to store all CPU state.
- All instruction parsing, execution, and memory updates are performed in strict accordance with the instruction set.
- Supports custom assembly files; easy to extend for new instructions.
- Minimal external libraries used: `<stdio.h>` and `<string.h>` only.
- Designed for clarity and educational demonstration of assembly language execution.

---

## Author

Muhammed Korkmaz  
Gebze Technical University  
Student ID: 220104004043

---

**For full instruction set and more examples, see `instructions 1.txt` and the provided PDF report.**
