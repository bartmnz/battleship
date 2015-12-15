#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


typedef struct{
    int column;
    int row;
    bool hit;
    char boatType;
} coordinate;

struct gameBoard{
    int size;
    int numCols;
    int numRows;
    int numSpaces;
    coordinate locations[19];
};
