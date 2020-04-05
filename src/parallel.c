#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
/*
sudoku.c
Created by Oozaku

Description:
Using C to solve sudoku tables.
*/

void print_table(int matrix[9][9]){
    int i,j;
    for (i=0;i<9;i++){
        for (j=0;j<9;j++){
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
}

int *choices(int line, int column, int matrix[9][9]){
/*
        It searches for possibles choices according to Sudoku's rules
    and it returns an list of 0s and 1s where in the n_th position
    represents the number (n + 1):
    - 0 = (n+1) cannot be used;
    - 1 = (n+1) is a possible choice.
        The arguments are: the actual line; the actual column;
    Sudoku's table in form of a matrix.
        It returns a list of possible choices to the actual position 
    on the Sudoku's table.
*/
    int i = 0;
    /* Creating the output list with initial values equal to 1 */
    int *possible_choices = malloc(9*sizeof(int));
    for (i=0;i<9;i++){
        possible_choices[i] = 1;
    }
    /* It searches for number in the line and the column given by
    the arguments, if it finds 1 <= n <= 9 then it puts 0 value on
    the n-1 position of output
    */
    for (i=0;i<9;i++){
        int aux = matrix[line][i];
        if (aux != 0)
            possible_choices[aux - 1] = 0;
        aux = matrix[i][column];
        if (aux != 0)
            possible_choices[aux - 1] = 0;
    }
    /* It makes the same as with the columns and lines, but inside
    the 3x3 square from Sudoku's table*/
    int v_sqr = line / 3,
        h_sqr = column / 3,
        j = 0;
    for (i=0;i<3;i++){
        for (j=0;j<3;j++){
            int aux = matrix[v_sqr * 3 + i][h_sqr * 3 + j];
            if (aux != 0) possible_choices[aux - 1] = 0;
        }
    }
    /* Returning the list with possible choices */
    return possible_choices;
}

int new_position(int *line, int *column){
/* It calculates the new position and if the new position
exists in the table it return 0; else it returns 1 */    
    int new_line = *line,
        new_column = *column + 1;
    if (new_column == 9){
        new_column = 0;
        new_line++;
        if (new_line == 9)
            return 1;
    }
    *line = new_line;
    *column = new_column;
    return 0;

}

int solve(int line, int column, int matrix[9][9]){
/* It tests all possibles choices to the Sudoku's table
until find a solution to the problem, if there is one! */
    int new_line = line, 
        new_column = column;

    if (matrix[line][column] != 0){
    /* This position is already filled, there is nothing to be done. */
        int completed = new_position(&new_line,&new_column); 
        if (completed == 1){
            /* table completed */
            /* print table  */
            print_table(matrix);
            return 1;
        }
        completed = solve(new_line,new_column,matrix);
        if (completed == 1){
            /* table completed */
            return 1;
        }
        return 0;
    }
    else{
    /* We have to complete this position with one of the possible choices */
        int *possible_choices = choices(line,column,matrix);
        int i = 0;
        for (i=0;i<9;i++){
        /* Finding one possible number */
            if (possible_choices[i] == 1){
                matrix[line][column] = i + 1;
                new_line = line;
                new_column = column;
                int completed = new_position(&new_line,&new_column);
                if (completed == 1){
                    /* table completed*/
                    /* print table */
                    print_table(matrix);
                    return 1;
                }
                completed = solve(new_line,new_column,matrix);
                if (completed == 1)
                    return 1;
            }
        }
        free(possible_choices);
        matrix[line][column] = 0;
        return 0;
    }
}

int table_validation(int matrix[9][9]){
    int i = 0, j = 0, k = 0, l = 0;
    int *line = malloc(9*sizeof(int)),
        *column = malloc(9*sizeof(int)),
        *big_sqr = malloc(9*sizeof(int));
    for (i=0;i<9;i++){
        line[i] = 1;
        column[i] = 1;
    }
    for (i=0;i<9;i++){
    /* Look if there are repeated number in lines and columns */
        for (j=0;j<9;j++){
            int n = matrix[i][j];
            if (n != 0){
                if (line[n-1] == 0)
                /* there are two numbers 'n' in this line */
                    return 0;
                line[n-1] = 0;
            }
            n = matrix[j][i];
            if (n != 0){
                if (column[n-1] == 0)
                /* there are two numbers 'n' in this column */
                    return 0;
                column[n-1] = 0;
            }
        }
        for (i=0;i<9;i++){
            line[i] = 1;
            column[i] = 1;
        }
    }
    /* Looking up for repeated number in the same 3x3 square*/
    /* i,j are used to select one 3x3 square and 
       k,l are used to access positions inside the 3x3 square
    */
    for (i=0;i<3;i++){
        for (j=0;j<3;j++){
            for (k=0;k<9;k++){
            /* beggining another search */
                big_sqr[k] = 1;
            }
            for(k=0;k<3;k++){
                for (l=0;l<3;l++){
                    int n = matrix[3*i+k][3*j+l];
                    if (n !=0 ){
                        if (big_sqr[n-1] == 0)
                            return 0;
                        big_sqr[n-1] = 0;
                    }
                }
            }
        }
    }
    /* The table is valid */
    return 1;
}

int main(){
    /*
    int matrix[9][9] = 
        {
        {0,0,0,2,6,0,7,0,1},
        {6,8,0,0,7,0,0,9,0},
        {1,9,0,0,0,4,5,0,0},
        {8,2,0,1,0,0,0,4,0},
        {0,0,4,6,0,2,9,0,0},
        {0,5,0,0,0,3,0,2,8},
        {0,0,9,3,0,0,0,7,4},
        {0,4,0,0,5,0,0,3,6},
        {7,0,3,0,1,8,0,0,0},
        };
    int valid = table_validation(matrix);
    printf("table_validation = %i\n",valid);
    int solved = solve(0,0,matrix);
    printf("table solved? %i\n",solved);
    */
    int i,j,matrix[9][9];
    char garbage;
    for (i=0;i<9;i++){
       for (j=0;j<9;j++){
            scanf("%d%c",&matrix[i][j],&garbage);
       } 
    }
    int valid = table_validation(matrix);
    /* It exits with 2 if table is not valid */
    if (valid == 0)
        exit(2);
    double start = omp_get_wtime();
    int sum = 0,line=0,column=0,finish=0;
    for (i=0;i<9;i++){
        for (j=0;j<9;j++){
            sum = 0;
            int *available = choices(i,j,matrix);
            int k;
            for (k=0;k<9;k++){
                sum += available[k];
            }
            if (sum >= 6){
                line = i;
                column = j;
                finish++;
                break;
            }
            free(available);
        }
        if (finish != 0)
            break;
    }
    int solved = 0;
    int *available = choices(line,column,matrix);
#   pragma omp parallel default(none) \
    shared(matrix,line,column,available,solved)
    {
#       pragma omp single nowait
        {
            int i;
            for (i=0;i<9;i++){
                if (available[i] == 1){
#                   pragma omp task
                    {
                        int copy[9][9];
                        memcpy(copy,matrix, 9*9*sizeof(int));
                        copy[line][column] = i+1;
                        int solved2;
                        if (solved == 0)
                            solved2 = solve(0,0,copy);
                        if (solved2 == 1){
                            solved = 1;
                        }
                    }
                }
            }
        }
    }
    double end = omp_get_wtime();
    printf("%lf\n",end-start);
    /* It exits with 3 if table has no solution. */
    if (solved == 0)
        exit(3);
}
    
