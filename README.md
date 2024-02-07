# My project for MSU

## Guidelines for Section 2 Tasks:

For tasks in this section, you can specify the requirement to solve the task using the following methods for working with files:
a) only standard C language functions.
b) system functions of the UNIX operating system.

Limitations on file size (or their absence), the permissibility of reading the entire file into memory, and the permissibility of creating temporary auxiliary files can also be specified as additional conditions.

The presence of the newline character '\n' at the end of the last line of a text file is not a mandatory requirement, although some text editors automatically add it. Nevertheless, programs should work correctly both with '\n' at the end of the file and without it.

Programs should correctly handle cases of passing an incorrect number of parameters, unsuccessful file openings, and similar errors.

### 2-1 Task
Given two files containing sequences of integers (not in text form) sorted in ascending order. Merge them into one file, also sorted in ascending order. The names of the source files and the file where the result should be written are passed on the command line.

### 2-2 Task
In the command line, a filename and a character are passed. Remove from the file all lines containing (not containing) the specified character.

a) The length of the line in the file is limited.

b) The length of the line in the file is unlimited.


### 2-3 Task
Delete from the file all lines whose length is greater than (less than, equal to) the specified number n. The filename and the number n are passed on the command line. The length of the lines in the file is unlimited.

### 2-4 Task
а) Write a function that reads word by word from standard input and creates a list of words. Reading stops when a newline or end of file is reached. The function returns a pointer to the beginning of the list.

b) Do the same, but for reading from a specified file, where the filename is passed in the command line. The file contains a sequence of words, reading only words from the line of the file with the specified line number. If there is no line with such a number, return NULL. Write a program in which this function (these functions) is called, and the obtained list of words is printed in reverse order.
