#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

#define ROWS 10
#define COLUMNS 10
#define BOMBS 10
#define PROB 0.08
#define CELL '+'
#define BOMB '@'

#define CURSOR '*'
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define PRESS 'p'

void init_board(); // initializes the board
void print_board();// prints the board on every iteration
void place_bombs();// places the bombs randomly till total reached using recursion
int count_neighbours(int a, int b);// counts neighbouring bombs of an empty cell 
void process_command(char command);// used for user interaction 
void start();// starts the game
void trigger_press(int a, int b); // checks what happens if user presses on certain cell
bool inside(int a, int b);//checks whether inside the grid or not 
int check_win();//checks if the user has won the game

char board[100][100], user_board[100][100];
bool uncovered_board[100][100];//checks whether a certain cell is covered or uncovered 
int i,j,finished=0, user_row,user_col;
int bomb_counter=0;

void init_board(){
    for(i=0; i<ROWS; i++){
        for(j=0; j<COLUMNS; j++)
            board[i][j] = user_board[i][j] = CELL;
    }
    place_bombs();
    for(i=0; i<ROWS; i++){
        for(j=0; j<COLUMNS; j++){
            if(board[i][j] != BOMB){
               board[i][j] = '0' + count_neighbours(i,j);
            }
        }
   }
}
void print_board(){
    for(i=0; i<ROWS; i++){
        for(j=0; j<COLUMNS; j++)
            printf("%c", user_board[i][j]);
        
        printf("\n");
    }
    printf("\nEnter Command: ");    
}

void place_bombs(){
    srand(time(0));
    for(i=0; i<ROWS; i++){
        for(j=0; j<COLUMNS; j++){
            float x = (rand()%100);
            float prob = x/100;
            if(bomb_counter == BOMBS) break;
            if(prob <= PROB && board[i][j] != BOMB){
                board[i][j] = BOMB;
                bomb_counter++;   
            }
        }
    }
    if(bomb_counter<BOMBS){ 
        place_bombs();//recursively calling the function again until required number of bombs are placed
    }    
}
int count_neighbours(int a, int b){
    int neighbouring_bombs = 0;

    for(int k1 = -1; k1 <= 1; k1++){//iterating through the 3x3 grid surrounding the cell for bombs
        for(int k2 = -1; k2 <= 1; k2++){
            if(board[a + k1][b + k2] == BOMB){
                neighbouring_bombs++;
            }
        }
    }
    return neighbouring_bombs;
}
void process_command(char command){
    int next_row = user_row, next_col = user_col;

    if(command == UP){
        next_row--;
    }else if(command == DOWN){
        next_row++;
    }else if(command == RIGHT){
        next_col++;
    }else if(command == LEFT){
        next_col--;
    }
    if(!inside(next_row, next_col))
        return;
    if(!uncovered_board[user_row][user_col])
        user_board[user_row][user_col] = CELL;
    else
        user_board[user_row][user_col] = board[user_row][user_col];
    user_row = next_row;
    user_col = next_col;
    user_board[user_row][user_col] = CURSOR;//moving the cursor to the next row/col where the user wants to go 
   

    if(command == PRESS){
        trigger_press(user_row, user_col);
    }

}
bool inside(int a, int b){
    if(a>= 0 && b >= 0 && a < ROWS && b< COLUMNS)
        return true;
    return false;
}
void trigger_press(int a, int b){
    if(uncovered_board[a][b]){//if pressed on an already uncovered cell we go back 
        return;
    }
    if(board[a][b] == BOMB){//if pressed cell contains a bomb we exit out of the game
        finished = 1;
        printf("\nGAME OVER\n");
        for(i=0; i<ROWS; i++){
            for(j=0; j<COLUMNS; j++){
                if(board[i][j] == BOMB){
                    printf("%c", BOMB);
                }else{
                    printf("%c", user_board[i][j]);
                    }
                }printf("\n");
            }
        }else{
            uncovered_board[a][b] = 1;
            user_board[a][b] = board[a][b];
            printf("%c", user_board[a][b]);
           
             if(user_board[a][b] == '0'){//if uncovered cell has 0 neighbouring bombs we uncover all its neighbours
                for(int k1 = -1; k1 <= 1; k1++){//Here that is done by running this function on all neighbouring cells 
                    for(int k2 = -1; k2 <= 1; k2++){
                        if(inside(a+k1, b+k2))
                            trigger_press(a+k1, b+k2);
                    }
                }                
            }
        }
}
void start(){
    user_col = user_row = 0;
    finished = 0;
    init_board();
    user_board[user_row][user_col] = CURSOR;
}
int check_win(){//checks if all cells other than bombs have been uncovered
    int flag=0; 
    for(i=0; i<ROWS; i++){
        for(j=0; j<COLUMNS; j++){
            if(uncovered_board[i][j])
                flag++;
        }
    }
    if (flag == ROWS*COLUMNS - BOMBS){
        printf("\nYOU WON!!");
        return 1;
    }
    return 0;
}
int main(){//runs a while loop asking user to enter a command on each iteration
    start();
    while(!finished){
        finished = check_win(); 
        printf("\n");
        printf("\e[1;1H\e[2J"); //ANSI for clearing screen
        print_board();
        char command;
        scanf("%c", &command);
        process_command(command);
    }
    return 0;
}