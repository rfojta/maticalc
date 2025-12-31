# Matrix Calculator

The Matrix Calculator is a small shell that implements its own simple language.  
It supports reading, printing, editing, arithmetic operations, and more.

Matrices are stored in a global array of size **10**, indexed from **0 to 9**.

---

## Features

- Store up to **10 matrices**
- Read matrices from input
- Print matrices
- Add matrices
- Multiply matrices
- Edit individual elements
- Apply scalar operations
- Transpose matrices
- Execute commands from a file or interactively

---

## Command Reference

Below is the full list of commands implemented in the source code.

### **h** — Help  
Displays the list of available commands.

### **r 1** — Read matrix  
Reads a matrix into slot `1` from the input stream.

### **p 1** — Print matrix  
Prints matrix `1`.

### **a 1 2** — Add matrices  
Adds matrix `2` to matrix `1` (in‑place).

### **m 1 2 3** — Multiply matrices  
Computes `1 × 2` and stores the result into matrix `3`.  
Matrix `3` is freed before being replaced.

### **i 1 i j** — Print item  
Prints the value at row `i`, column `j` of matrix `1`.

### **e 1 i j x** — Edit item  
Sets the value at row `i`, column `j` of matrix `1` to `x`.

### **+ 1 x** — Add scalar  
Adds scalar `x` to every element of matrix `1`.

### **\* 1 x** — Multiply by scalar  
Multiplies every element of matrix `1` by scalar `x`.

### **t 1** — Transpose matrix  
Transposes matrix `1` in place.

### **# comment** — Comment  
Lines starting with `#` are ignored until the end of the line.

### **q** — Quit  
Exits the program.

---

## Input Modes

### **Interactive mode**
Run without arguments: