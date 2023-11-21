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


//global variables
struct cell {
    int position;
    int adj_count;
    int mined;
    int covered;
    int flagged;
    
};
typedef struct cell cell;

cell **board; //2-d dynamically allocated array of cells
int rows;
int cols;
int mines;


void command_new(){
    board = (cell **) malloc(sizeof(cell *) * rows);
    for (int i = 0; i<rows; i++){
        board[i] = (cell *) malloc(sizeof(cell) * cols);
    }
    
    for(int i =0; i<rows; i++){
        for (int j=0; j<cols; j++){
            board[i][j].position = i*cols + j;
            printf("%d\n", board[i][j].position);
        }
    }
}

void display_cell(cell *c){
//    printf("%4d", c->position);
    printf("%3s", "/");
}

void command_show(){
    for(int i = 0; i<rows; i++){
        for(int j = 0; j< cols; j++){
            display_cell(&board[i][j]);
        }
        printf("\n");
    }
}


#define MAXCHARLENGTH 80
#define MAXTOKENS 20
int run_game(){
    
    char line[MAXCHARLENGTH];
    char line_copy[MAXCHARLENGTH];
    char tokens[MAXTOKENS][MAXCHARLENGTH];
    char *tok;
    int token_count;
    
    
    
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





