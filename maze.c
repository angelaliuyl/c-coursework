#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define MAXDIM 20
#define MAXSTRLEN (MAXDIM + 1)
#define EMPTY ' '
#define FNLEN 100

typedef enum state {path, block, out} state;
typedef enum bool {false, true} bool;

void test(void);
/* given grid of maze and starting point, returns true if solution found */
/* this function modifies the original grid */
bool nextstep(char grid[MAXDIM][MAXSTRLEN], int row, int col);
state checkstate(char grid[MAXDIM][MAXSTRLEN], int row, int col);
void fillgrid(char filename[FNLEN], char grid[MAXDIM][MAXSTRLEN], int *p1, int *p2);
void printgrid(char grid[MAXDIM][MAXSTRLEN], int h);
bool getstart(char grid[MAXDIM][MAXSTRLEN], int *p1, int *p2);

int main(int argc, char **argv)
{
    char filename[FNLEN];
    char grid[MAXDIM][MAXSTRLEN] = {{0}};
    int x_start, y_start, height, width;

    test();

    if (argc == 2) {
        strcpy(filename, argv[1]);
        fillgrid(filename, grid, &height, &width);
        printf("height = %d, width = %d\n", height, width);
        printgrid(grid, height);
        if(!getstart(grid, &y_start, &x_start)) {
            printf("Starting point of maze not found\n");
            exit(EXIT_FAILURE);
        }
        /* printf("coordinate of start = (%d, %d)\n", x_start, y_start); */
        if (nextstep(grid, y_start, x_start)) {
            printf("Solution found: \n");
            printgrid(grid, height);
            return 0;
        }
        else {
            printf("Solution not found\n");
            return 1;
        }

    }
    else {
        printf("Incorrect usage, try %s maze.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    return 0;
}

void test(void)
{
    char grid[MAXDIM][MAXSTRLEN]= {
        "##########",
        "       #.#",
        "# ###### #",
        "#   #    #",
        "# # # ## #",
        "# ### #  #",
        "# #   ## #",
        "# # #  # #",
        "#   #  # #",
        "######## #"
    };
    int y_start = 1, x_start = 0;

    assert(checkstate(grid, 1, 8) == block);
    assert(checkstate(grid, 0, 0) == block);
    assert(checkstate(grid, 3, 2) == path);
    assert(checkstate(grid, -1, 1) == out);
    assert(checkstate(grid, 2, -1) == out);

    /* assert(nextstep(grid, y_start, x_start)); */

}

bool nextstep(char grid[MAXDIM][MAXSTRLEN], int row, int col)
{
    static int steps = 0;
    state down, up, left, right;

    /*
    printf("Step %d, row = %d, col = %d\n", steps, row, col);
    printgrid(grid, 10);
    */

    down = checkstate(grid, row + 1, col);
    up = checkstate(grid, row - 1, col);
    left = checkstate(grid, row, col - 1);
    right = checkstate(grid, row, col + 1);

    /* printf("Paths d:%d u:%d l:%d r:%d\n", down, up, left, right); */

    /* check if at exit */
    if (down == out || up == out || left == out || right == out) {
        /* making sure not at beginning */
        if (steps > 0) {
            grid[row][col] = '*';
            return true;
        }
    }

    grid[row][col] = '.';
    steps++;

    /* check down */
    if (down == path)
    {
        if(nextstep(grid, row + 1, col)) {
            return true;
        }
    }
    /* check up */
    if (up == path)
    {
        if(nextstep(grid, row - 1, col) == true) {
            return true;
        }
    }
    /* check left */
    if (left == path)
    {
        if(nextstep(grid, row, col - 1) == true) {
            return true;
        }
    }
    /* check right */
    if (right == path)
    {
        if(nextstep(grid, row, col + 1) == true) {
            return true;
        }
    }

    return false;

}

state checkstate(char grid[MAXDIM][MAXSTRLEN], int row, int col)
{
    int s = MAXDIM;
    /* check if out of bound */
    if (row < 0 || row >= s || col < 0 || col >= s) {
        return out;
    }
    /* return block if wall or explored */
    if (grid[row][col] == '#' || grid[row][col] == '.') {
        return block;
    }
    if (grid[row][col] == EMPTY) {
        return path;
    }
    return out;
}

void printgrid(char grid[MAXDIM][MAXSTRLEN], int h)
{
    int y;
    for (y = 0; y < h; y++) {
        printf("%s", grid[y]);
    }
}

void fillgrid(char filename[FNLEN], char grid[MAXDIM][MAXSTRLEN], int *p1, int *p2)
{
    FILE *fp;
    int i = 0;
    char str[MAXSTRLEN];
    bool finished = false;

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("Cannot open file\n");
        exit(EXIT_FAILURE);
    }

    if ((fscanf(fp, "%d %d\n", p1, p2)) != 2) {
        printf("Incorrect file formats\n");
        exit(EXIT_FAILURE);
    }

    do{
        finished = false;
        /* EOF is reached */
        if (fgets(str, MAXSTRLEN, fp) == NULL) {
            finished = true;
        }
        else {
            strcpy(grid[i], str);
            i++;
        }
    } while (!finished);

    fclose(fp);

}

bool getstart(char grid[MAXDIM][MAXSTRLEN], int *p1, int *p2)
{
    int y, x;

    for(y = 0; y < MAXDIM; y++) {
        for(x = 0; x < MAXDIM; x++) {
            if (grid[y][x] == ' ') {
                *p1 = y;
                *p2 = x;
                return true;
            }
        }
    }

    return false;
}
