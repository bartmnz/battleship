#include "battleShip.h"

int saveBoard( struct gameBoard*, char*);
int makeBoard( struct gameBoard*, int );
int addShip( struct gameBoard*, int, char);
int goodValue(struct gameBoard*, int*, int*, int);
void checkGuess(struct gameBoard*, int, int);
int shipsLeft(struct gameBoard*);
int loadGame(struct gameBoard*, char*);

int main (void){
    srand(time(NULL));
    struct gameBoard* theBoard = malloc( sizeof(struct gameBoard));
    makeBoard(theBoard, 10);
    saveBoard(theBoard, "test.txt");
//    while(shipsLeft(theBoard) != 0 ){
        char c[4];
        fgets(c, 4, stdin);
        int row = atoi(c);
        fgets(c, 4, stdin);
        int col = atoi(c);
        checkGuess(theBoard, row, col);
        saveBoard(theBoard, "results.txt");
//        printf("ships left = %d \n", shipsLeft(theBoard));
//    }
    struct gameBoard* otherBoard = malloc( sizeof(struct gameBoard));
    loadGame(otherBoard, "test.txt");
    saveBoard(otherBoard, "LoadCheck.txt");
    free(otherBoard);
    free(theBoard);
}

/* Function builds a battleship board. 
 * Parameter Size is expected to be between 10 and 100, is the size of the board
 * Parameter fileName is the name of the file to store the board in.
 * Return 1 on success else 0
 */

int makeBoard( struct gameBoard* theBoard, int size){
    if (size < 10 || size > 100){
        fprintf(stderr, "ERROR: expected size between 10 and 100. \n");
        return 0;
    }
    theBoard->size = size;
    char boats[6] = {'P', 'S', 'C', 'D', 'B', 'A'};
    int lens[6] = {2, 2, 3, 3, 4, 5};
    for(int i = 0; i < 6; i ++){
        if (!addShip(theBoard, lens[i], boats[i])){
            fprintf( stderr, "ERROR: Could not add ships. \n");
            return 0;
        }
    }
   return 1;
}

/*Function to add a ship to a gameBoard
 *Parameter theBoard -- the game board being used
 *Parameter length the size of the ship being added
 *Parameter type -- special character to identify ship on board
 *Return 1 on success else 0
 */
int addShip( struct gameBoard* theBoard, int length, char type){
    // check if gameboard is null
    
    bool isPlaced = false;
    int* col = malloc(length), *row = malloc(length), dir;
    while(!isPlaced){
        // need to initialize col and row to zero;
        col[0] = rand() % theBoard->size;
        row[0] = rand() % theBoard->size;
        dir = rand() % 4;               // choose a starting place and direction
        
        if( dir == 0){                  // set arrays to correspond to potential ship placement
            for(int j = 1; j < length; j++){
                col[j] = col[j-1]+1;
                row[j] = row[j-1];
            }
        }
        if( dir == 1){
            for(int j = 1; j < length; j++){
                col[j] = col[j-1]-1;
                row[j] = row[j-1];
            }
        }
        if( dir == 2){
            for(int j = 1; j < length; j++){
                col[j] = col[j-1];
                row[j] = row[j-1]+1;
            }
        }
        if( dir == 3){
            for(int j = 1; j < length; j++){
                col[j] = col[j-1];
                row[j] = row[j-1]-1;
            }
        }
        if (goodValue(theBoard, col, row, length)){
            isPlaced = true;
        } 
    }
    int index = theBoard->numSpaces;
    for( int i = 0; i < length; i++){
        theBoard->locations[index + i].row = row[i];
        theBoard->locations[index + i].column = col[i];
        theBoard->locations[index + i].hit = false;
        theBoard->locations[index + i].boatType = type;
    }
    theBoard->numSpaces += length;
    free(col);
    free(row);
    
    return 1;
}


int goodValue(struct gameBoard* theBoard, int col[], int row[], int length){
    for( int i = 0; i < length; i ++){
            if(col[i] >= theBoard->size || row[i] >= theBoard->size
                || col[i] < 0 || row[i] < 0){
                return 0;                  // position is out of bounds.
            }
            for ( int count = 0; count < theBoard->numSpaces; count ++){
                if( theBoard->locations[count].row == row[i] &&
                    theBoard->locations[count].column == col[i]){
                    return 0;          // location intersects with a ship.
                }
            }
            if (i+1 == length){          // is a valid area to place ship
                return 1;
            }
        }
    return 0;
}

/*Function to save the gameBoard to a file
 *Parameter fileName, name of File to save board to
 *Parameter theBoard game board to save
 *Return 1 on success else 0
 */
int saveBoard( struct gameBoard* theBoard, char* fileName){
    FILE* file;
    if( ! (file = fopen( fileName, "w"))){
        fprintf( stderr, "ERROR: Could not open file.\n");
        return 0;
    }
//    if ( theBoard == NULL){
//        fprintf( stderr, "ERROR: could not save board. \n");
//        return 0;
//    }
    int size = 0;
    size = theBoard->size;
    if (!(size > 9 && size < 101)){
        fprintf( stderr, "ERROR: invalid size. \n");
        return 0;
    }
    char** temp;                                // temporary matrix to display board contents as actual board
    temp = malloc( 88);
    for( int i = 0; i <= size; i ++){
        temp[i] = malloc(sizeof(char) * (size));
    }
    for (int i = 0; i <= size; i++){
        for (int j =0; j < size; j++){
            temp[i][j] = '*';    
        }
    }
//    memset(temp, 0, sizeof(*temp));
    
    for ( int count = 0; count < 19; count ++){ // for each location, set temp to appropiate value.
//        printf(" %d, %d = %c\n", theBoard->locations[count].column, theBoard->locations[count].row, theBoard->locations[count].boatType);
        temp[(theBoard->locations[count].row)+1][theBoard->locations[count].column] = theBoard->locations[count].boatType;
        
    }
        int columns, rows;
//    printf("9,5 is %c\n ", temp[9][5]);
    for ( rows = 1; rows <= theBoard->size; rows ++){
        for ( columns = 0; columns < theBoard->size; columns ++){
          // printf(" %c ", temp[columns][rows]);   
            fwrite( &(temp[rows][columns]), 1, 1, file );
        }
       // printf("\");
        fwrite("\n", 1, 1, file);
    }
    
    for (int i = 0; i <= size; i++){
        free (temp[i]);
    }
    free(temp);
    fclose(file);
    return 1;
}

/* Function to check if an ordered pair is a hit or miss
 * Parameter theBoard board to evaluate.
 * Parameter Column - x value of the ordered pair to guess
 * Parameter Row -- y value of the ordered pair to guess
 * Return 1 on success else 0
 */
 
void checkGuess(struct gameBoard* theBoard, int row, int column){
    for ( int count = 0; count < theBoard->numSpaces; count ++){
                if( theBoard->locations[count].row == row-1 &&
                    theBoard->locations[count].column == column-1){
                    theBoard->locations[count].boatType = 'X';
                    fprintf(stdout, "HIT\n");          // location intersects with a ship.
                    return;
                }
            }
    fprintf(stdout, "MISS\n");
}

/* Function evaluates the board to see how many ships are left
 * Parameter theBoard -- the game board to be evaluated.
 */

int shipsLeft(struct gameBoard* theBoard){
    bool hasType[6] = {false, false, false, false, false, false};
    
    for ( int count = 0; count < theBoard->numSpaces; count ++){
        if(!( theBoard->locations[count].hit)){
            if( theBoard->locations[count].boatType == 'P'){
                hasType[0] = true;
            } else if( theBoard->locations[count].boatType == 'S'){
                hasType[1] = true;
            } else if( theBoard->locations[count].boatType == 'C'){
                hasType[2] = true;
            } else if( theBoard->locations[count].boatType == 'D'){
                hasType[3] = true;
            } else if( theBoard->locations[count].boatType == 'B'){
                hasType[4] = true;
            } else if( theBoard->locations[count].boatType == 'A'){
                hasType[5] = true;
            } 
        }
    }
    int rValue = 0;
    for( int i = 0; i < 6; i++){
        if( hasType[i]){
            rValue++;
        }
    }
    return rValue;
}

/*Function opens a saved game and loads the data into theBoard
 * Parameter fileName -- name of file to open
 * Parameter theBoard -- game to store data from file.
 * Return 1 on success else 0
 */

int loadGame(struct gameBoard* theBoard, char* fileName){
    FILE* file;
    if( ! (file = fopen( fileName, "r"))){
        fprintf( stderr, "ERROR: Could not open file.\n");
        return 0;
    }
    memset(theBoard, 0, sizeof (struct gameBoard));
    char temp; 
    int column = 0;
    int row = 0;
    while ((temp = fgetc(file)) != EOF){
        printf(" column %d row %d size %d\n", column, row, theBoard->numSpaces);
       // fgetc(stdin);
        if ( temp == '\n'){
            row ++;
            theBoard->size = row;
            column = 0;
            continue;
        }
        if (temp != '*'){
            if( temp == 'P' || temp == 'S' || temp == 'C' || temp == 'D'
                || temp == 'B' || temp == 'A' || temp =='X'){
                    theBoard->locations[theBoard->numSpaces].column = column;
                    theBoard->locations[theBoard->numSpaces].row = row;
                    theBoard->locations[theBoard->numSpaces].hit = false;
                    theBoard->locations[theBoard->numSpaces].boatType = temp;
                    theBoard->numSpaces++;
            } else {
                fprintf(stderr, "ERROR: Invalid boat Type. \n");
                return 0;
            }
        }
        column++;
    }
    return 1;
}