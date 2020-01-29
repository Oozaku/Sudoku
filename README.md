# Sudoku

Creating a linux application to solve sudoku table using C and Bash code 
to create the solver (sudoku.c) and the interface of usage (solve.h)
, respectively.

To use it make sure that solve.sh has execute permission with 
`chmod u+x solve.sh`.

You must have at least one text file with the table, where 0 represents
blank spaces and the numbers of the same line are separeted with a space.
Follow the 'example' file's formating. 

Then you can solve one or more tables with:
`./solve.sh example`

And for more than one, for example 3:
`./solve.sh example1 example2 example3`

The solution is saved at the same path of solve.sh and the name is 
name-of-file-solution. For example if we execute `./solve.sh example`
the output is "example-solution".
