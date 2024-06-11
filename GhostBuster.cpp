#include "GhostBuster.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

// Currently rows and columns are set to 8, however your game implementation should work for any other number
const int rows = 8, cols = 8;

// grid will store characters matrix of rows*cols size, you have to allocate memory to it in initialize function below
char* grid = NULL;

// Ghost will be placed over this location
int ghostRow, ghostCol;

// ***********************************
// No change zone: Don't change anything until the next stars line
// ***********************************

// Don't Change it: Screen width and height values
const int width = 1000, height = 600;

// Don't Change: This function is dealing with SDL and currently it's complete. Don't change anything here
void drawOneBlock(SDL_Renderer* renderer, SDL_Texture* texture, int row, int col, char sym){
    int xbox = width/cols;
    int ybox = height/rows;
    SDL_Rect src;
    switch(sym){
        case 'L': src = {141, 7, 104, 113}; break;
        case 'S': src = {427, 12, 129, 103}; break;
        case 'T': src = {268, 10, 130, 111}; break;
        case 'B': src = {0, 3, 114, 111}; break;
        case 'G': src = {571, 0, 123, 121}; break;
        case 'Z': src = {571, 0, 25, 25}; break;
        case 'F': src = {707, 16, 90, 86}; break;
    }
    SDL_Rect mov = { xbox*col, ybox*row, xbox - 10, ybox - 10};
    SDL_RenderCopy(renderer, texture, &src, &mov);
}

void drawBlocks(SDL_Renderer* renderer, SDL_Texture* texture){
    // Call drawOneBlock for all of the blocks of grid
    // for example to draw a snake over block (3, 5) you call it this way:
    // drawOneBlock(renderer, texture, 3, 5, 'S');
    if(grid == NULL) return;
    for(int i=0;i<rows; i++){
        for(int j=0;j<cols; j++){
            drawOneBlock(renderer, texture, i, j, grid[i*cols+j]);
        }   
    }
}

// ***********************************
// No change zone ends here
// ***********************************
// To Do zone: 
bool ended = false;

int ybox = height/rows, xbox = width/cols; 

void initialize(){ 
    //  To get Lock in the whole grid
    int table = rows*cols;
    grid = new char[table]; 
    int i=0;
    do{
        *(grid + i) = 'L';
        i++;
    }while(i <= table+1);

    // Call ghost at random coordinates
    ghostRow = (rand() % rows)+1;    
    ghostCol = (rand() % cols)+1;
}

void random_no_generator(int n, int block){//function to generate random number
    int randomly = rand() % n; 
        if(randomly == 2)
        {                             // condiiton for random number == 2
            *(grid + block) = 'S';   // placing 'S' for snake
        }
        else if(randomly == 1)
        {                             // condiiton for random number == 1
            *(grid + block) = 'T';   // placing 'T' for turtle
        }
        else if(randomly == 0)
        {                            // condiiton for random number == 0
            *(grid + block) = 'B';  // placing 'B' for bunny 
        }
}
void huntGhost(int x, int y){ 
    // this function is called every time you click on the screen
    // x, y are screen coordinates, you need to identify the block from these coordinates and using screen width and height values
    // each block size is width/cols x height/rows
    if(ended == false)
    {
       // getting row number column number and appropriate area of square where we click
        int noOfRow = y/ybox+1, noOfColumn = x/xbox+1; 
        // area
        int fullArea, unrequiredArea;
        fullArea = cols * noOfRow;
        unrequiredArea = cols - noOfColumn;
        int block = fullArea - unrequiredArea - 1;          
        // distance formula
        int x1, x2, y1, y2;
        x1 = ghostRow;
        x2 = noOfRow;
        y1 = ghostCol;
        y2 = noOfColumn;
        int distance = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
        // for randomnly denerating the sprite
        if(distance > 4)
        {
            random_no_generator(3, block);
        }
        //  for bunny
        if(distance == 4)
        {
            *(grid + block) = 'B';  
        }
        // for turtle
        else if(distance == 3 || distance == 2)
        {
            *(grid + block) = 'T'; 
        }
        //  for snake
        else if(distance == 0 || distance == 1)
        {
            *(grid + block) = 'S'; 
        }
    }
}

void bustGhost(int x, int y){ // this function is called when you right-click
    
    if(ended == false)
    {
       // getting row number column number and appropriate area of square where we click
        int noOfRow, noOfColumn;
        noOfRow = y / ybox + 1;
        noOfColumn = x / xbox + 1; 

        // area
        int fullArea, unrequiredArea;
        fullArea = cols * noOfRow;
        unrequiredArea = cols - noOfColumn;
        int block = fullArea - unrequiredArea - 1;    

        // for fail
        if (ghostRow != noOfRow && ghostCol != noOfColumn)
        {
            *(grid + block) = 'F'; 
            cout<<"YOU FAIL!"<<endl;
            ended = true;
        }
        // for win
        else if (ghostRow == noOfRow && ghostCol == noOfColumn)
        {
            *(grid + block) = 'G'; 
            cout<<"YOU WON!"<<endl;
            ended = true;
        }
    }
}

void quitGhostBuster(){ 
    // deleting the grid    
    delete [] grid;
}