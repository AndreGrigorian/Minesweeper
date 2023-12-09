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
#include <ctype.h>


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




void game_reset(){
    board = NULL;
    rows = 0;
    cols = 0;
    mines = 0;
}


int get_random(int range) {
    return rand() % ((range-1) - 0 + 1)+0; // (max-min+1) + min
}

int is_valid_cell(int r, int c){
    if((r <= rows && r>0) && (c <= cols && c>0)) return 1;
    else return 0;
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


void init_cell(cell *c, int p) {
    c->position = p; // each cell knows its pos in board
    c->has_mine = 0; // mined initially false
    c->adj_count = 0; // adjcount initially 0
    c->covered = 1; // covered initially true
    c->flagged = 0; // flagged initially false
}

void command_new(){
    if(board != NULL){
        free(board);
    }
    board = (cell **) malloc(sizeof(cell *) * rows);
    for (int i = 0; i<rows; i++){
        board[i] = (cell *) malloc(sizeof(cell) * cols);
    }
    
    for(int i =0; i<rows; i++){
        for (int j=0; j<cols; j++){
            init_cell(&board[i][j],i*cols + j );
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


void display_cell(cell *c) {
    if(c->flagged == 1) printf("%3s", "P");
    else if(c->covered == 1) printf("%3s", "/");
    else if (c->has_mine == 1) printf("%3s", "*");
    else if (c->adj_count > 0) printf("%3d", c->adj_count);
    else printf("%3s", ".");
}

void display_cell_answers(cell *c) {
    if (c->has_mine == 1) printf("%3s", "*");
    else if (c->adj_count == 0) printf("%3s", ".");
    else printf("%3d",c->adj_count);
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
        }
        printf("\n");
    }
    
    
    //----debugging stuff----//
    
    printf("Answers\n");
    printf(" ");
    for(int i = 0; i<= cols; i++){
        printf("%3d", i);
    }
    printf("\n");
    
    for(int i = 0; i<rows; i++){
        printf("%3d|", i+1); //print row numbers
        for(int j = 0; j< cols; j++){
            display_cell_answers(&board[i][j]);
        }
        printf("\n");
    }
}



void check_win(){
    for(int i =0; i< rows;i++){
        for(int j =0; j<cols; j++){
            if(board[i][j].has_mine && !board[i][j].flagged){
                return;
            }
            else if (!board[i][j].has_mine && board[i][j].covered){
                return;
            }
        }
    }
    printf("You Won!!\n");
    game_reset();
}

void command_flag(int r, int c){
    if(is_valid_cell(r, c)){
        board[r-1][c-1].flagged = 1;
        check_win();
    }
    else{
        printf("%s\n", "index out of bounds");
    }
}


void command_unflag(int r, int c){
    if(is_valid_cell(r, c)){
        board[r-1][c-1].flagged = 0;
    }
    else{
        printf("%s\n", "index out of bounds");
    }
}



void uncover_recursive(int r, int c){
    int rowneighbors[] = {-1,-1,+0,+1,+1,+1,+0,-1};
    int colneighbors[] = {+0,+1,+1,+1,+0,-1,-1,-1};
    
    board[r][c].covered = 0;
    check_win();
    
    if(board[r][c].adj_count == 0){
        for(int i=0; i<8;i++){
            int rn = r + rowneighbors[i];
            int cn = c + colneighbors[i];
            if((rn<rows && rn>=0) && (cn<cols && cn>=0)){ //check if rn and cn are out of bounds
                if(board[rn][cn].covered && !board[rn][cn].has_mine){
                    uncover_recursive(rn, cn);
                }
            }
        }
    }
}

void command_uncover(int r, int c){
    if(is_valid_cell(r, c)){
        r = r-1;
        c = c-1;
        if(board[r][c].has_mine == 1){//hit mine
            printf("%s\n", "You Lost!!");
            game_reset();
        }
        else if(board[r][c].adj_count > 0){//adj count >0
            board[r][c].covered = 0;
            check_win();
        }
        else if (board[r][c].adj_count == 0){//adj count =0
            uncover_recursive(r, c);
        }
    }
    else{
        printf("%s\n", "index out of unbounds");
    }
}


int all_digits(char *s){
    int len = strlen(s);
    for (int i=0; i<len; i++) {
        if (!isdigit(s[i])) return 0;
    }
    return 1;
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
            if(all_digits(tokens[1]) && all_digits(tokens[2]) && all_digits(tokens[3]) && token_count == 4){
                printf("%s\n", "New baord created");
                rows = atoi(tokens[1]);
                cols = atoi(tokens[2]);
                mines = atoi(tokens[3]);
                command_new();
//                if(board != NULL){
//                    free(board);
//                }else{
//                    command_new();
//                }
            }else{
                printf("invalid arguments passed\n");
            }
            
        }
        else if(strcmp(tokens[0], "show") == 0){
            //show command
            command_show();
        }
        else if(strcmp(tokens[0], "uncover") == 0){
            if(all_digits(tokens[1]) && all_digits(tokens[2]) && token_count == 3){
                command_uncover(atoi(tokens[1]), atoi(tokens[2]));
            }else{
                printf("invalid arguments passed\n");
            }
        }
        else if(strcmp(tokens[0], "flag") == 0){
            //flag
            if(all_digits(tokens[1]) && all_digits(tokens[2]) && token_count == 3){
                command_flag(atoi(tokens[1]), atoi(tokens[2]));
            }else{
                printf("invalid arguments passed\n");
            }
        }
        else if(strcmp(tokens[0], "unflag") == 0){
            //unflag command
            if(all_digits(tokens[1]) && all_digits(tokens[2]) && token_count == 3){
                command_unflag(atoi(tokens[1]), atoi(tokens[2]));
            }else{
                printf("invalid arguments passed\n");
            }
        }
        else{
            printf("unknown command\n");
        }
        
    }
   
    return 1;
}

int main(int argc, const char * argv[]) {
    run_game();
    return 0;
}




