
# Assembler
A limited custom-built assembly compiler written in c-90.

# Abstract


## Fatal Errors Assumptions

In case of fatal errors, the program will terminate gracefully close the program at once. These include but are not limited to:

- Inability to perform IO operations
- Memory allocation failures
- Any other critical errors that impede the program's functionality
- input lines are expected to contain at most 80 characters


## Assumptions

This program follows the 2024 1st semester guidelines provided by the Israeli Open University the course forums, and assumptions made by the team:
- Input lines contain at most `80` characters (+1 for string termination)
- There is a limit to the **amount of node allocations** that are used by the tables/maps within this program
- Macros content can be assumed to be correct
- Macros always have a closing tag (`endmcr`)
- Macros definition always appear prior to their usage
- Macros context are encapsulated
- There is a fixed amount of operations that need to be supported which do not cover the entire assembly lang operations
- There is no requirement to use the most optimal solutions for the data-structures (black-red-trees/hash-tables), as such we took the liberty of using a sorted list instead
- An operand has a maximal length of 36 characters (we preferred to reduce allocations for easier stability and maintenance)
- The maximal amount of operands is `80`
- Input macro names are defined once (distinct macros) 
- Parse failures only prevent the creation of output files, however they do not break the flow
  - Fatal errors such as inability to perform IO operations or memory allocations will gracefully close the program at once
- We use a small amount of static globals in self-contained modules (namely the log handling)
- Errors must be printed to `stderr`
- Normal logs are printed to `stdout`
- The program is tested for ubuntu 16.04, Windows 11, ubuntu 20.0

# Installation
##
```shell
make
```
# Uninstallation
```shell
rm -rf LOCAL_PROJECT_PATH
```

# Usage

To run the program, make sure you have installed it properly first, following that use the following commands in your prefered terminal:
```shell
./assembler ./inputs/YOUR_DESIRED_INPUT
```

If you wish to store error logs into a file, please execute the program and pipe the `stderr` into a file as follows:
```shell
./assembler ./inputs/YOUR_DESIRED_INPUT 2> PATH_TO_ERROR_FILE
```

## Inputs
Under the inputs sub-folder you can find many input files. As a convention valid inputs begin with the letter `h` (for "happy"), and invalid inputs begin with the letter `i` (for "invalid")
>NOTE: per the project requirements, the outputs of the assembler are written there as well.
<br/>

Below is a short overview of the inputs:
<br />

Valid inputs:
- `h1.as` is the post pre-compilation file that was provided as a legitimate example by the university
- `h1-plus-macro.as` is the same as `h1.as` where a few instructions are written as a valid macro.
- `h2.as` is another example provided by the university
- `h3.as` includes `h2.as` with a couple other additions

Invalid inputs:
- `i1.as` includes various illegitimate instructions (nonexistent/supported operators), directives (non-existing directives, bad format, and bad operands), operands (reserved names, and invalid formats), comments etc.
  - This is the primary errors file
- `i2.as` is an invalid input which contains enough data assignments that cross the `MAX_ALLOWED_ALLOCATIONS (4096)`
- `i3.as` demonstrates an invalid macro, where the macro name is followed by extra text
- `i4.as` demonstrates a macro which is missing a macro name
- `i4.as` demonstrates forbidden macro names

# License
This is the property of Shay Hatan, and Itay Alperovich, the project is strictly made for academic purposes.
You are not allowed to use this tool for commercial purposes without an approval from the authors.
The authors do not take any responsibility for the damages that may incur to you if you run this program.
If you wish to use it in your own public project, please credit the authors for their contribution.