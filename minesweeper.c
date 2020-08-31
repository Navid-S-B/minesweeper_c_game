// Assignment 1 20T1 COMP1511: Minesweeper
// minesweeper.c

// This program was written by Navid Bhuiyan
// on 2020-02-19

/*
PROGRAM DESCRIPTION

This program plays an altered version of minesweeper using the terminal to play
the game. This game is currently at Stage 3, where it has a gameplay mode
with simple features. It can decide if you won or lost, and can provide three
hints on where mines are possibly located.

General Abbreviations Key:
    - col : column

This game has yet to implement successfully all of Stage 4 features.
It has managed to implement reveal_radial but not first_turn. See
first turn function for extra detail.

Bugs:
    - Stage 4 first_turn function only works with one mine.
Need to Fix:
    - Add proper Stage 4 first_turn function.
*/

// Version 1.0.0 (2020-03-08): Assignment released.
// Version 1.0.1 (2020-03-08): Fix punctuation in comment.
// Version 1.0.2 (2020-03-08): Fix second line of header comment to say minesweeper.c
// Version 1.0.3 (2020-03-25): Successful implementation and documentation of Stage 1, 2 and 3. 
// Version 1.0.4 (2020-03-26): Added better variable names/comments for functions.
// Version 1.0.5 (2020-03-28): Added restrict_hints for execute_function.
// Version 1.0.6 (2020-03-29): Partially added Stage 4 (reveal_radial - full, first_turn - partially).

#include <stdio.h>
#include <stdlib.h>

// Possible square states.
#define VISIBLE_SAFE    0
#define HIDDEN_SAFE     1
#define HIDDEN_MINE     2

// The size of the starting grid.
#define SIZE 8

// The possible command codes.
#define DETECT_ROW              1
#define DETECT_COL              2
#define DETECT_SQUARE           3
#define REVEAL_SQUARE           4
#define GAMEPLAY_MODE           5
#define DEBUG_MODE              6
#define REVEAL_RADIAL           7

// Add any extra #defines here.
#define IS_NULL -100 // For NULL values (so they have int value).
#define WIN 2
#define CONTINUE 1
#define LOST 0
#define REVEAL_SIZE 3 // Used for reveal_square
#define PRINT_ON 1 // To turn on print statements.
#define PRINT_OFF !PRINT_ON // To turn off print_statements.
#define TRUE 1 // For valid_index function.
#define FALSE !TRUE // For valid_index function.

void initialise_field(int minefield[SIZE][SIZE]);
void print_debug_minefield(int minefield[SIZE][SIZE]);
int decide_win(int centre_row_dw, int centre_col_dw, int minefield[SIZE][SIZE]);
void detect_row(int row, int minefield[SIZE][SIZE]);
void detect_column(int col, int minefield[SIZE][SIZE]);
int valid_index(int row_vi, int col_vi);
void reveal_square_helper(int centre_row_rsh, int centre_col_rsh, int minefield[SIZE][SIZE]);
int detect_square(int print_indicator, int centre_row_ds, int centre_col_ds, int square_size, int minefield[SIZE][SIZE]);
int reveal_square(int centre_row_rs, int  centre_col_rs, int minefield[SIZE][SIZE]);
int restrict_hints(int no_hints, int input_1_rh, int input_2_rh, int input_3_rh, int input_4_rh, int minefield[SIZE][SIZE]);
void gameplay_mode_VISIBLE_SAFE(int adjacent_mines, int column_counter);
void gameplay_mode_HIDDEN_MINE(int column_counter);
void gameplay_mode_HIDDEN_SAFE(int column_counter);
void gameplay_mode(int win_status, int minefield[SIZE][SIZE]);
void decide_gameplay_debug_mode(int input_1_IS_NULL, int win_status, int mode_choice, int minefield[SIZE][SIZE]);
void radial_blast(int centre_row_rb, int  centre_col_rb, int minefield[SIZE][SIZE]);
int reveal_radial(int centre_row_rr, int  centre_col_rr, int minefield[SIZE][SIZE]);
void first_turn(int centre_row_ft, int centre_col_ft, int minefield[SIZE][SIZE]); // Doesn't work fully. 
int execute_function(int gameplay_debug_mode, int hint_counter, int input_1, int input_2, int input_3, int input_4, int minefield[SIZE][SIZE]);

// PROTOTYPE FUNCTIONS

// Function handles user inputs and translates it by inputting it into an 
// execute function.
int main(void) {

    int minefield[SIZE][SIZE];
    initialise_field(minefield);
    printf("Welcome to minesweeper!\n");
    printf("How many mines? ");

    // Scan in the number of pairs of mines.
    
    int no_mines = IS_NULL;
    scanf("%d", &no_mines);    
    
    printf("Enter pairs:\n");
    
    // Scan in the pairs of mines and place them on the grid.
    
    int loop_counter = 0;
    int temp_rows = 0;
    int temp_cols = 0;
    
    while (loop_counter < no_mines) {
    
        scanf("%d %d", &temp_rows, &temp_cols);
        
        if (temp_cols < SIZE && temp_rows < SIZE) {
        
            if (temp_cols >= 0 && temp_rows >= 0) {
                
                minefield[temp_rows][temp_cols] = HIDDEN_MINE;
            
            }
        
        }

        loop_counter++;
    
    }

    printf("Game Started\n");
    print_debug_minefield(minefield);

    // Initialising loop variables.
    int if_continue = CONTINUE; // Variable which breaks the loop.
    int user_int_1 = IS_NULL; // First digit user enters.
    int user_int_2 = IS_NULL; // Second digit user enters.
    int user_int_3 = IS_NULL; // Third digit user enters.
    int user_int_4 = IS_NULL; // Fourth digits user enters.
    int no_hints = 0; // Hint counter to limit user to only have 3 hints. 
    int gameplay_debug_mode = DEBUG_MODE; // Minefield printing mode variable.
    // int no_first_turns = 0; // To limit first_turn to only first_turn.
    
    // While loop which continues forever unless win, loss or EOF (Crtl-D).
    // Scans for a number of integers based on user input then
    // uses a function to execute the intended function and print output
    // based on a certain user input. Always prints the minefield
    // after each command.
    while (if_continue == CONTINUE && scanf("%d", &user_int_1) == 1) {
    
        if (user_int_1 == DETECT_ROW || user_int_1 == DETECT_COL)  {
        
            scanf(" %d", &user_int_2);
            no_hints++;
            
        } else if (user_int_1 == DETECT_SQUARE) {
            
            scanf(" %d %d %d", &user_int_2, &user_int_3, &user_int_4);
            no_hints++;
        
        } else if (user_int_1 == REVEAL_SQUARE || user_int_1 == REVEAL_RADIAL) {
            
            scanf(" %d %d", &user_int_2, &user_int_3);
            
            // Disable this for Stage 3  to completely pass.
            // Placed function here instead of execute_function, is because
            // I wanted to modify the minefield before any other function is 
            // executed.
            
            /*if (minefield[user_int_2][user_int_3] == HIDDEN_MINE && no_first_turns < 1) {
                
                // Disable this for Stage 3 complete passing.
                first_turn(user_int_2, user_int_3, minefield);
                no_first_turns++;
            
            }*/
        
        } else {// When the user is selecting the minefield mode.
        
            gameplay_debug_mode = user_int_1;
            user_int_1 = IS_NULL;
        
        }
        
        // Execute function interprets user outputs and runs the intended 
        // commands. It returns an integer which decides when to break 
        // the while loop i.e. when a player loses or wins.
        // Refer to execute_function for an explanation of variables.
        if_continue = execute_function(gameplay_debug_mode, no_hints, 
                                    user_int_1, user_int_2, user_int_3, 
                                    user_int_4, minefield);   
        
    }
    
    return 0;
}


// Set the entire minefield to HIDDEN_SAFE. _if -> intialise_field
void initialise_field(int minefield[SIZE][SIZE]) {
    
    int row_if = 0;
    int col_if = 0;
    
    while (row_if < SIZE) {
        
        while (col_if < SIZE) {
            
            minefield[row_if][col_if] = HIDDEN_SAFE;
            
            col_if++;
        
        }
        
        col_if = 0;
        row_if++;
    }
}


// Print out the actual values of the minefield. _dm -> debug minefield.
void print_debug_minefield(int minefield[SIZE][SIZE]) {
    
    int row_dm = 0;
    int col_dm = 0;
    
    while (row_dm < SIZE) {
        
        while (col_dm < SIZE) {
            
            printf("%d ", minefield[row_dm][col_dm]);
            col_dm++;
        }
        
        printf("\n");
        col_dm = 0;
        row_dm++;
    }
}


// A helper function for reveal_square to check if there is a winning minefield.
// Scans through whole minefield to detect any HIDDEN_SAFE to change the 
// assumption of a win to no win. _dw -> decide win.
int decide_win(int centre_row_dw, int centre_col_dw, int minefield[SIZE][SIZE]) {

    // Assume that a win has occured.
    int if_win = WIN;
    
    int row_dw = 0;
    int col_dw = 0;

    while (row_dw < SIZE && if_win != CONTINUE) {

        while (col_dw < SIZE) {
        
            if (minefield[row_dw][col_dw] == HIDDEN_SAFE) {
                
                if_win = CONTINUE; // No win yet if there is HIDDEN_SAFE
            
            }
             
            col_dw++;
        }
        
        col_dw = 0;
        row_dw++;
    
    }

    
    if (if_win == WIN) {
            
        printf("Game Won!\n");
    
    }    

    return if_win;
    
}


// Stage 1
// Loops through the minefield to count the number of HIDDEN_MINE's in a 
// selected row.
void detect_row(int selected_row, int minefield[SIZE][SIZE]) {
    
    int col_counter = 0;
    int mine_counter = 0;
    int if_mine = IS_NULL;
    
    while (col_counter < SIZE) {
        
        if_mine = minefield[selected_row][col_counter];
        
        if (if_mine == HIDDEN_MINE) {
            mine_counter++;
        }
                
        col_counter++;
            
    }
    
    printf("There are %d mine(s) in row %d\n", mine_counter, selected_row);
    

}


// Stage 1
// Loops through the minefield to count the number of HIDDEN_MINE's in a 
// selected column.
void detect_column(int selected_col, int minefield[SIZE][SIZE]) {

    int row_counter = 0;
    int mine_counter = 0;
    int assume_mine = IS_NULL;

    while (row_counter < SIZE) {
        
        assume_mine = minefield[row_counter][selected_col];
        
        if (assume_mine == HIDDEN_MINE) {
           
            mine_counter++;
            
        }
                
        row_counter++;
            
    }
    
    printf("There are %d mine(s) in column %d\n", mine_counter, selected_col);

}


// Stage 2
// Counts the number of mines in a square of any size, which is valid inside the
// minefield. It does this by only allowing valid index values to be inputted
// into the array to check for HIDDEN MINE's. It returns the number of mines
// in a given square as an integer.
// TO NOTE: print_indicator is a variable which turns off printing a string
// when the user is not using detect_square directly.
// TO NOTE: ds -> detect_square
int detect_square(int print_indicator,  int centre_row_ds, int centre_col_ds, 
                int square_size, int minefield[SIZE][SIZE]) {

    // Number which substracts to the row and column index to the top right
    // corner of the square.    
    int subs_to_corner = square_size/2;
    
    // Coordinates for the top left hand corner of square_size*square_size square.
    int top_left_corner_row_ds = centre_row_ds - subs_to_corner; 
    int top_left_corner_col_ds = centre_col_ds - subs_to_corner;

    // Counter variables and position variables.
    int row_counter_ds = 0;
    int col_counter_ds = 0;
    int mine_counter_ds = 0;
    int row_position_ds = IS_NULL; // Used to reduce character count. 
    int col_position_ds = IS_NULL; // Used to reduce character count.
  
    
    // While loop starts off at the top left corner of the square,
    // which may or may not be in bounds of the minefield. However, the while
    // loop only allows coordinates inside the minefield to detect
    // mines.
    
    while (row_counter_ds < square_size) {
 
        while (col_counter_ds < square_size) {
            
            row_position_ds = top_left_corner_row_ds + row_counter_ds;     
            col_position_ds = top_left_corner_col_ds + col_counter_ds;
            
            int if_valid_index = valid_index(row_position_ds, col_position_ds);
        
            if (if_valid_index == TRUE) {// Index check to avoid error.
                
                if (minefield[row_position_ds][col_position_ds] == HIDDEN_MINE) {
                    
                    mine_counter_ds++;
                
                } 
                
            }
            
            col_counter_ds++; // Change column. 
    
        }

        col_counter_ds = 0; // Reset column counter.
        row_counter_ds++; // Change row.
    
    }

    // If the user calls detect_square directly then print this output.
    if (print_indicator == PRINT_ON) {
        
        printf("There are %d mine(s) in the square centered at row %d, column %d of size %d\n", mine_counter_ds, centre_row_ds, centre_col_ds, square_size);
    
    }
    
    return mine_counter_ds;

}

// Stage 2
// This is a helper function for reveal_square.
// This is is to zero a 3*3 square with it's indices valid inside the 
// minefield.
void reveal_square_helper(int centre_row_rsh, int centre_col_rsh, 
                            int minefield[SIZE][SIZE]) {
    
    // Coordinates on top of 3x3 square for while loop to decipher.
    // Allows for row and collumn while loop deciphering.
    int top_left_corner_row_rsh = centre_row_rsh - 1; 
    int top_left_corner_col_rsh = centre_col_rsh - 1;
    int row_counter_rsh = 0;
    int col_counter_rsh = 0;
    int row_position_rsh = IS_NULL; // Final row position on minefield.
    int col_position_rsh = IS_NULL; // Final column position on minefield.
    
    while (row_counter_rsh < REVEAL_SIZE) {
                
        while (col_counter_rsh < REVEAL_SIZE) {
            
            row_position_rsh = top_left_corner_row_rsh + row_counter_rsh;    
            col_position_rsh= top_left_corner_col_rsh + col_counter_rsh;
            
            int if_valid_index = valid_index(row_position_rsh, col_position_rsh);
                        
            if (if_valid_index == TRUE) {// Index check
                
                minefield[row_position_rsh][col_position_rsh] = VISIBLE_SAFE;
                
            }
            
            col_counter_rsh++; // Change column. 
    
        }

        col_counter_rsh = 0; // Reset column counter.      
        row_counter_rsh++; // Change row.
    
    }   


}


// Stage 2
// A function which enables the user to reveal a 3*3 square. 
// It is similar to how detect_square works, it does not count anything,
// rather it replaces valid indices in the minefield.
// TO NOTE: rs -> reveal_square
int reveal_square(int centre_row_rs, int  centre_col_rs, int minefield[SIZE][SIZE]) {

    // Find the number of total mines in the square.
    int no_mines = detect_square(PRINT_OFF, centre_row_rs, centre_col_rs, 
                                REVEAL_SIZE, minefield);

    if (no_mines == 0) {
        
        // Refer to reveal_square_helper.
        reveal_square_helper(centre_row_rs, centre_col_rs, minefield);
    
    } else {// Assuming valid input and terminate game is player has lost.
    
        if (minefield[centre_row_rs][centre_col_rs] != HIDDEN_MINE) {
        
            minefield[centre_row_rs][centre_col_rs] = VISIBLE_SAFE;
                 
        } else {// Selects mine.
        
            printf("Game over\n");
            return LOST;
        
        }
        
    }

    // Helper function to decide if the player has won or not.
    int if_winner_rs = decide_win(centre_row_rs, centre_col_rs, minefield);
    
    return if_winner_rs;
    
}


// Stage 3
// Restrict hints takes a hint counter from the main function, and then
// based on the hint count, execute all of the hint functions i.e.
// detect_row, detect_col and detect_square. 
// TO NOTE: rh -> restrict_hints.
int restrict_hints(int no_hints, int input_1_rh, int input_2_rh, int input_3_rh, 
                    int input_4_rh, int minefield[SIZE][SIZE]) {
    
    if (no_hints <= 3) {
        
        if (input_1_rh == DETECT_ROW) {
            
            detect_row(input_2_rh, minefield);
        
        } else if (input_1_rh == DETECT_COL) {
        
            detect_column(input_2_rh, minefield);        
        
        } else {
            
            detect_square(PRINT_ON, input_2_rh, input_3_rh, input_4_rh, 
                         minefield);
              
        }

    } else {
    
        printf("Help already used\n");
    
    }
    
    // returns CONTINUE to keep the game continuing.
    return CONTINUE;
}


// Stage 3
// A helper function for gameplay_mode which prints the adjacent mine values 
// e.g. "01" when value on the debug_mode is equal to VISIBLE_SAFE.
void gameplay_mode_VISIBLE_SAFE(int adjacent_mines, int column_counter) {

    // Places adjacent mines values with appropriate formatting.
    if (adjacent_mines > 0 && column_counter == 7) {

        printf("%02d", adjacent_mines);

    } else if (adjacent_mines > 0 && column_counter != 7) {

        printf("%02d ", adjacent_mines); 
        
    } else if (adjacent_mines == 0 && column_counter == 7) {

        printf("  ");

    } else {// adjacent_mines == 0 and collumn_counter != 7.
        
        printf("   ");
    }

}


// Stage 3
// A helper function for gameplay_mode which prints "()" when value on the 
// debug_mode is equal to HIDDEN_MINE.
void gameplay_mode_HIDDEN_MINE(int column_counter) {

    // Apply proper formatting according the column the function is printing.
    if (column_counter == 7) {
                
        printf("()");
        
    } else {
        
        printf("() "); 
    
    }

}


// Stage 3
// A helper function for gameplay_mode which prints "##" when value on the 
// debug_mode is equal to HIDDEN_SAFE.
void gameplay_mode_HIDDEN_SAFE(int column_counter) {

    if (column_counter == 7) {

        printf("##");
        
    } else {

        printf("## "); 

    }        

}


// Stage 3
// Prints the gameplay mode of the minefield. Depending on the game "win_status".
// Applies () modification to hidden mines.
// Cannot further reduce nesting, otherise readability is further compromised.
void gameplay_mode(int win_status, int minefield[SIZE][SIZE]) {
    
    // Initialising index counters for number to ## grid for gamplay mode (gp).
    int gp_row_counter = 0;
    int gp_col_counter = 0;
    int adj_mines = IS_NULL; // adjacent_mines
    
    // Always print the smiley face until the game is lost.
    if (win_status == CONTINUE || win_status == WIN) {
        
        printf("..\n");
        printf("\\/\n");
    
    } else {

        printf("xx\n");
        printf("/\\\n");

    }
    
    // Print first two lines of gameplay mode matrix.
    printf("    00 01 02 03 04 05 06 07\n");
    printf("   -------------------------\n");  

    while (gp_row_counter < SIZE) {
        
        // Printing each row number first.
        printf("%02d |", gp_row_counter);
        
        while (gp_col_counter < SIZE) {
            
            // Conditions to print certain output for debug_mode values
            // i.e. VISIBLE_SAFE, HIDDEN_SAFE and HIDDEN_SAFE.
            // Also prints HIDDEN_MINE's as () when game is lost.
            if (minefield[gp_row_counter][gp_col_counter] == VISIBLE_SAFE) {
                
                // Number of adjacent mines to input into VISIBLE_SAFE value.
                adj_mines = detect_square(PRINT_OFF, gp_row_counter, 
                                        gp_col_counter, REVEAL_SIZE, minefield);
                
                // Input "0X" with corrent formatting in gameplay_mode. 
                gameplay_mode_VISIBLE_SAFE(adj_mines, gp_col_counter);
                
            } else if (minefield[gp_row_counter][gp_col_counter] == HIDDEN_MINE 
                        && win_status == LOST) {
                
                // Input "()" with correct formatting in gameplay mode.
                gameplay_mode_HIDDEN_MINE(gp_col_counter);
                            
            } else {
            
                
                // Input "##" with correct formatting in gamplay mode.
                gameplay_mode_HIDDEN_SAFE(gp_col_counter);
                
            }
            
            // Increase the column counter.
            gp_col_counter++;
            
        }
        
        printf("|\n");    
        gp_row_counter++;
        gp_col_counter = 0;
           
    }
        
    // Print final line of gameplay mode.
    printf("   -------------------------\n");            
    
}


// Function which changes the mode depending on the choice of the user.
// A helper function for the execute_function, gameplay_mode and debug_mode.
void decide_gameplay_debug_mode(int input_1_IS_NULL, int win_status, 
                                int mode_choice, int minefield[SIZE][SIZE]) {
    
    // input_1_IS_NULL is used to determine whether to print the statements
    // each time each mode is initialised.
    
    if (mode_choice == GAMEPLAY_MODE) {
    
        if (input_1_IS_NULL == IS_NULL) {
        
            printf("Gameplay mode activated\n");
        
        }
        
        gameplay_mode(win_status, minefield);
    
    } else {
    
    
        if (input_1_IS_NULL == IS_NULL) {
        
            printf("Debug mode activated\n");
        
        }
    
        print_debug_minefield(minefield);
    
    }

}


// A helper function for both radial_blast and Stage 2 functions.
// It sieves out indices which are not inside the minefield, so
// no input errors occur.
int valid_index(int row_vi, int col_vi) {

    int valid = FALSE;
    
    if (row_vi >= 0 && row_vi <=7) {
    
        if (col_vi >= 0 && col_vi <= 7) {
        
            valid = TRUE;
        
        }
    
    }
    
    return valid;

}


// Stage 4
// A helper function for reveal_square. radial_blast individually reveals 
// each angled blast at a time. 
// TO NOTE: rb -> radial_blast
void radial_blast(int centre_row_rb, int  centre_col_rb, 
                int minefield[SIZE][SIZE]) {
    
    // Indices which start of each angled blast at a certain angle.
    int row_index_0 = centre_row_rb - 1;
    int col_index_0 = centre_col_rb;
    int row_index_45 = centre_row_rb - 1;
    int col_index_45 = centre_col_rb + 1;
    int row_index_90 = centre_row_rb;
    int col_index_90 = centre_col_rb + 1;
    int row_index_135 = centre_row_rb + 1;
    int col_index_135 = centre_col_rb + 1;
    int row_index_180 = centre_row_rb + 1;
    int col_index_180 = centre_col_rb;
    int row_index_225 = centre_row_rb + 1;
    int col_index_225 = centre_col_rb - 1;
    int row_index_270 = centre_row_rb;
    int col_index_270 = centre_col_rb - 1;
    int row_index_315 = centre_row_rb - 1;
    int col_index_315 = centre_col_rb - 1;
    
    // Used to stop the blast from going on if there are mines besides the 
    // blast.
    int mines_exist = 0;
    
    
    // Each while loop is similar in terms of its structure.
    // It works by starting at the corner of the reveal square, and then
    // extends the blast until a mine is adjacent to its path.
    // The valid index function in the while loop guard, is to ensure
    // the blast stops once the blast is outside the minefield,
    // to have no array input error.
    while (valid_index(row_index_0, col_index_0) == TRUE && mines_exist == 0) {
        
        mines_exist = detect_square(PRINT_OFF, row_index_0, col_index_0, 
                                    REVEAL_SIZE, minefield);
        
        // This terminates the while loop and allows for the end blast to 
        // stop at an adjacent mine.
        if (mines_exist != 0 && minefield[row_index_0 - 1][col_index_0] != HIDDEN_MINE) {
            
            minefield[row_index_0][col_index_0] = 0;
         
        } else {// continues the blast fowards.
        
            minefield[row_index_0][col_index_0] = 0;
            row_index_0--;
        
        }
        
    } 
    
    // Reset mine_counter for the next while loop.
    mines_exist = 0;
    
    while (valid_index(row_index_45, col_index_45) == TRUE && mines_exist == 0) {
        
        mines_exist = detect_square(PRINT_OFF, row_index_45, col_index_45, 
                                    REVEAL_SIZE, minefield);
        
        if (mines_exist != 0 && minefield[row_index_45 - 1][col_index_45 + 1] != HIDDEN_MINE) {
            
            minefield[row_index_45][col_index_45] = 0;
         
        } else {
        
            minefield[row_index_45][col_index_45] = 0;
            row_index_45--;
            col_index_45++;
        
        }
        
    }
    
    
    mines_exist = 0;
    
    while (valid_index(row_index_90, col_index_90) == TRUE && mines_exist == 0) {
        
        mines_exist = detect_square(PRINT_OFF, row_index_90, col_index_90,
                                   REVEAL_SIZE, minefield);
        
        if (mines_exist != 0 && minefield[row_index_90][col_index_90 + 1] != HIDDEN_MINE) {
            
            minefield[row_index_90][col_index_90] = 0;
         
        } else {
        
            minefield[row_index_90][col_index_90] = 0;
            col_index_90++;
        
        }
        
    }
     
    
    mines_exist = 0;
     
    while (valid_index(row_index_135, col_index_135) == TRUE && mines_exist == 0) {
        
        mines_exist = detect_square(PRINT_OFF, row_index_135, col_index_135, 
                                    REVEAL_SIZE, minefield);
        
        if (mines_exist != 0 && minefield[row_index_135 + 1][col_index_135 + 1] != HIDDEN_MINE) {
            
            minefield[row_index_135][col_index_135] = 0;
         
        } else {
        
            minefield[row_index_135][col_index_135] = 0;
            row_index_135++;
            col_index_135++;
        
        }
        
    }
     
    
    mines_exist = 0;
    
    while (valid_index(row_index_180, col_index_180) == TRUE && mines_exist == 0) {
        
        mines_exist = detect_square(PRINT_OFF, row_index_180, col_index_180, 
                                    REVEAL_SIZE, minefield);
        
        if (mines_exist != 0 && minefield[row_index_180 + 1][col_index_180] != HIDDEN_MINE) {
            
            minefield[row_index_180][col_index_180] = 0;
         
        } else {
        
            minefield[row_index_180][col_index_180] = 0;
            row_index_180++;
        
        }
        
    } 
    
    
    mines_exist = 0;
    
    while (valid_index(row_index_225, col_index_225) == TRUE && mines_exist == 0) {
        
        mines_exist = detect_square(PRINT_OFF, row_index_225, col_index_225,
                                    REVEAL_SIZE, minefield);
        
        if (mines_exist != 0 && minefield[row_index_225 + 1][col_index_225 - 1] != HIDDEN_MINE) {
            
            minefield[row_index_225][col_index_225] = 0;
         
        } else {
        
            minefield[row_index_225][col_index_225] = 0;
            row_index_225++;
            col_index_225--;
            
        }
        
    }
     
    
    mines_exist = 0;
    
    while (valid_index(row_index_270, col_index_270) == TRUE && mines_exist == 0) {
        
        mines_exist = detect_square(PRINT_OFF, row_index_270, col_index_270,
                                    REVEAL_SIZE, minefield);
        
        if (mines_exist != 0 && minefield[row_index_270][col_index_270 - 1] != HIDDEN_MINE) {
            
            minefield[row_index_270][col_index_270] = 0;
         
        } else {
        
            minefield[row_index_270][col_index_270] = 0;
            col_index_270--;
        
        }
        
    } 
    
    
    mines_exist = 0;
    
    while (valid_index(row_index_315, col_index_315) == TRUE && mines_exist == 0) {
        
        mines_exist = detect_square(PRINT_OFF, row_index_315, col_index_315,
                                    REVEAL_SIZE, minefield);
        
        if (mines_exist != 0 && minefield[row_index_315 - 1][col_index_315 - 1] != HIDDEN_MINE) {
            
            minefield[row_index_315][col_index_315] = 0;
         
        } else {
        
            minefield[row_index_315][col_index_315] = 0;
            row_index_315--;
            col_index_315--;
        
        }
        
    }
    
}


// Stage 4
// This reveal_radial function decides when a user wins or not based 
// on the reveal square function. It utilises radial_blast, to reveal_radial
// the 8-point blast based on surrounding mines.
int reveal_radial(int centre_row_rr, int centre_col_rr, int minefield[SIZE][SIZE]) {
    
    // Utilising reveal_square to see if the game is won/lost.
    int if_winner_loser_1 = reveal_square(centre_row_rr, centre_col_rr, 
                                            minefield);
    
    // Used to decide return output.
    int if_winner_loser_2 = IS_NULL; 
    
    // Used to only reveal the selected spot if there is mines surrounding it
    int adjacent_mines = detect_square(PRINT_OFF, centre_row_rr, 
                                                centre_col_rr, REVEAL_SIZE, 
                                                minefield);
    
    // Logic to decide return output for execute_function to continue
    // or end the game.
    if (if_winner_loser_1 == WIN) {
        
        if_winner_loser_2 = WIN;  
        
    } else if (if_winner_loser_1 == LOST) {
        
        if_winner_loser_2 = LOST;
    
    } else if (adjacent_mines != 0) {
        
        if_winner_loser_2 = CONTINUE;
    
    } else {
    
        radial_blast(centre_row_rr, centre_col_rr, minefield);
        if_winner_loser_2 = CONTINUE;
    
    }
    
    return if_winner_loser_2;

}


// Stage 4
// Only works with one mine/one mine column because this function works on
// moving single mines downwards than shifting the whole array downwards.
// TO NOTE: ft -> first_turn.
void first_turn(int centre_row_ft, int centre_col_ft, int minefield[SIZE][SIZE]) {

    int row_ft = 0;
    int col_ft = 0;    
        
    while (minefield[centre_col_ft][centre_col_ft] == HIDDEN_MINE) {
        
        while (col_ft < SIZE) {
        
            if (minefield[row_ft][col_ft] == HIDDEN_MINE) {
                    
                if (row_ft != 7) {
                
                    minefield[row_ft][col_ft] = HIDDEN_SAFE;
                    minefield[row_ft + 1][col_ft] = HIDDEN_MINE;
                
                } else {
                    
                    minefield[row_ft][col_ft] = HIDDEN_SAFE;
                    minefield[0][col_ft] = HIDDEN_MINE;
                
                }
                
            }
            
            col_ft++;
        
        }
        
        col_ft = 0;
        
        row_ft++;
        
        if (row_ft == 8) {
        
            row_ft = 0;
        
        }
        
    } 

}


// This function is a helper function of the main function and 
// reads the user inputs and relays them to the correct function
// for each stage.
int execute_function(int gameplay_debug_mode, int hint_counter, int input_1, 
                    int input_2, int input_3, int input_4, 
                    int minefield[SIZE][SIZE]) { 
    
    int loop_break = IS_NULL; // Used to carry return input.
    
    if (input_1 == IS_NULL) {
        
        loop_break = CONTINUE;            
    
    } else {
        
        if (input_1 == DETECT_ROW || input_1 == DETECT_COL || input_1 == DETECT_SQUARE) {
            
            // Helper function to activate Stage 1 functions with hint 
            // restriction.
            loop_break = restrict_hints(hint_counter, input_1, input_2, input_3,
                                        input_4, minefield);
        
        } else if (input_1 == REVEAL_SQUARE) {
        
            // reveal_square decides if game is won or not.
            loop_break = reveal_square(input_2, input_3, minefield);
        
        } else {
            
            // reveal_radial decieds if the game is won or not.
            loop_break = reveal_radial(input_2, input_3, minefield);        
        
        }
    
    }
    
    decide_gameplay_debug_mode(input_1, loop_break, gameplay_debug_mode, 
                                minefield);             
    
    return loop_break;
    
}
