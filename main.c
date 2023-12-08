//
//  main.c
//  Minesweeper222
//
//  Created by Andre Grigorian on 9/24/23.
//

// how to compile and build c file from terminal
// >> gcc main.c
// >> ./a.out



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


//global variables
struct cell {
    int position;
    int adj_count;
    int mined;
    int covered;
    int flagged;
    int has_mine; // 0 for no, 1 for yes
    
};
typedef struct cell cell;

cell **board; //2-d dynamically allocated array of cells
int rows;
int cols;
int mines;



int get_random(int range) {
    return rand() % ((range-1) - 0 + 1)+0; // (max-min+1) + min
}

void generate_mines(){
    
    for(int i = 0; i< mines; i++){
        int random_row = get_random(rows);
        int random_col = get_random(cols);
        while(board[random_row][random_col].has_mine == 1){
            random_row = get_random(rows);
            random_col = get_random(cols);
            printf("%d %d", random_row, random_col);
        }
        board[random_row][random_col].has_mine = 1;
    }
}



void command_new(){
    free(board);
    board = (cell **) malloc(sizeof(cell *) * rows);
    for (int i = 0; i<rows; i++){
        board[i] = (cell *) malloc(sizeof(cell) * cols);
    }
    
    for(int i =0; i<rows; i++){
        for (int j=0; j<cols; j++){
            board[i][j].position = i*cols + j;
//            printf("%d\n", board[i][j].position);
        }
    }
    generate_mines();
    
    
    int neighborcount = 8;
    int rowneighbors[] = {-1,-1,+0,+1,+1,+1,+0,-1};
    int colneighbors[] = {+0,+1,+1,+1,+0,-1,-1,-1};
    
    
    for(int i =0; i<rows; i++){
        for (int j=0; j<cols; j++){
            for (int d=0; d<neighborcount; d++) {
                int rn = i + rowneighbors[d]; // row for direction d
                int cn = j + colneighbors[d]; // col for direction d
                if (0 <= rn && rn < rows && 0 <= cn && cn < cols){
                    if (board[rn][cn].has_mine == 1){
                        board[i][j].adj_count++;
                    }
                }
            }
        }
    }
    
}

//void display_cell(cell *c){
//    printf("%3s", "/");
//}

void display_cell(cell *c) {
    if(c->flagged == 1) printf("%3s", "f");
    if (c->has_mine == 1) printf("%3s", "*");
    else if (c->adj_count == 0) printf("%3s", ".");
    else printf("%3d",c->adj_count);
}

void init_cell(cell *c, int p) {
    c->position = p; // each cell knows its pos in board
    c->has_mine = 0; // mined initially false
    c->adj_count = 0; // adjcount initially 0
    c->covered = 0; // covered initially false
    c->flagged = 0; // flagged initially false
}

void command_show(){
    
    //print column numbers
    printf(" ");
    for(int i = 0; i<= cols; i++){
        printf("%3d", i);
    }
    printf("\n");
    
    for(int i = 0; i<rows; i++){
        printf("%3d|", i+1); //print row numbers
        for(int j = 0; j< cols; j++){
            display_cell(&board[i][j]);
//            printf("%3d", board[i][j].adj_count);
        }
        printf("\n");
    }
}


void command_flag(int r, int c){
    printf("%d %d", r, c);
    board[r][c].flagged = 1;
}




#define MAXCHARLENGTH 80
#define MAXTOKENS 20
int run_game(){
    
    char line[MAXCHARLENGTH];
    char line_copy[MAXCHARLENGTH];
    char tokens[MAXTOKENS][MAXCHARLENGTH];
    char *tok;
    int token_count;
    
    srand(time(NULL));
    
    

    
    while(1){
        
        //get next command
        printf(">> ");
        fgets(line, MAXCHARLENGTH, stdin);
        line[strlen(line)-1] = '\0';
        
        //break commands into tokens (tokenizing)
        strcpy(line_copy, line);
        tok = strtok(line_copy, " ");
        token_count = 0;
        while(tok != NULL){
            strcpy(tokens[token_count], tok);
            tok = strtok(NULL, " "); //get next token
            token_count++;
        }
        
        //process next command
        
        if(strcmp(tokens[0], "quit") == 0) break;
        
        else if(strcmp(tokens[0], "new") == 0){
            //new command
            printf("%s\n", "New baord created");
            rows = atoi(tokens[1]);
            cols = atoi(tokens[2]);
            mines = atoi(tokens[3]);
            command_new();
            
        }
        else if(strcmp(tokens[0], "show") == 0){
            //show command
            command_show();
        }
        else if(strcmp(tokens[0], "uncover") == 0){
            //uncover command
            
        }
        else if(strcmp(tokens[0], "flag") == 0){
            //flag
            command_flag(atoi(tokens[1]), atoi(tokens[2]));
        }
        else if(strcmp(tokens[0], "unflag") == 0){
            //unflag command
        }
        
    }
   
    

//    printf("%lu/n", strlen(cmd));
//    printf("Hello, World!\n");
    return 1;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    run_game();
    return 0;
}




