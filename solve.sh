#!/bin/sh

# solve.sh
# Created by Oozaku

# Description:
# Interface of Sudoku's table solver, it compiles the sudoku.c
# program and controls the input and output of the program

# If solver's executable does not exist, create one
if ! [ -f sudoku ];then
    gcc sudoku.c -o sudoku
fi

# If compilation goes wrong, print message before exiting
if ! [ $? -eq 0 ];then
    # If sudoku.c does not exist
    if ! [ -f sudoku.c ];then
        printf "ERROR: sudoku.c does not exist at the actual path\n. Try downloading sudoku.c in https://github.com/Oozaku/Sudoku and save it in $(pwd).\n"
    # Another kind of error...
    else
        echo "ERROR: something went wrong with sudoku.c compilation!"
    fi
    exit 1
fi

# It sees if there are arguments, if there are no arguments it 
# exist with error. 
if [ $# -eq 0 ];then
    echo "ERROR - input files were not given!"
    exit 1
fi

for i in $@
do
    # If file "$i" does not exist, the program exits with error. 
    if ! [ -f "$i" ];then
        printf "File '$i' does not exist.\nExiting...\n"
        exit 1
    fi

    # Executing solver and save output
    output="$(./sudoku < $i)"
    
    # Saving exit signal from  solver...
    buffer=$?

    # Creating output file's name
    output_file_name="$(basename "$i")-solution"
    
    # If everything goes well, errors=0; otherwise
    # errors=1
    errors=0

    # Looking for errors executing solver
    if ! [ $buffer -eq 0 ];then
        errors=1
        if [ $buffer -eq 2 ];then
            echo "Table '$i' is not valid!"
            echo "Invalid table" > $output_file_name
        elif [ $buffer -eq 3 ];then
            echo "Table '$i' has no solution"
            echo "No solution found" > $output_file_name
        else
            echo "Another type of error occured, check '$i''s formatation.."
        fi
    fi
    
    # If everuthing went well to $i, then save the solution
    if [ $errors -eq 0 ];then
        echo "$output" > $output_file_name
    fi

done

echo "Eding the program..."
