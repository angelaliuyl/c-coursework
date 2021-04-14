#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "neillsimplescreen.h"

#define WIDTH 80
#define HEIGHT 30
#define G 250
#define L (10 * G)
#define GENERATIONS 5000
#define EMPTYCHAR ' '
#define TREECHAR '@'
#define FIRECHAR '*'
#define DEFAULT empty
#define WAITTIME 0.05
#define INITSTATE empty

enum state {empty, tree, fire};
typedef enum state state;
enum bool {false, true};
typedef enum bool bool;
struct displaytext {
    char text; neillcol colour;
};
typedef struct displaytext displaytext;

/* fill a grid with one input */
void fillGrid(state grid[HEIGHT][WIDTH], state fill);
/* computes next generation given a grid of states */
void evolution(state g1[HEIGHT][WIDTH], state g2[HEIGHT][WIDTH]);
/* computes next gen of a tree state given position on grid */
state evoTree(state grid[HEIGHT][WIDTH], int row, int col);
/* computes next gen of an empty state */
state evoEmpty(void);
/* is there a fire in an adjacent tile? */
bool isFireNear(state grid[HEIGHT][WIDTH], int row, int col);
/* computes text and colour corresponding to a state */
displaytext stateToDisplay(state s);
/* are g1 and g2 the same? */
bool gridMatch(state g1[HEIGHT][WIDTH], state g2[HEIGHT][WIDTH]);
/* copies value of g2 to g1 */
void gridCopy(state g1[HEIGHT][WIDTH], state g2[HEIGHT][WIDTH]);
/* given divisor, returns true 1 time in divisor */
bool chance(int div);
/* reads from grid an empty value if out of bounds */
state safeRead(state g[HEIGHT][WIDTH], int row, int col);
/* prints a grid onto the screen */
void gridPrint(state g[HEIGHT][WIDTH]);
/* counts the number of a state given a grid */
int countState(state g[HEIGHT][WIDTH], state s);
/* tests functions */
void test(void);

int main(void)
{
    state previous_grid[HEIGHT][WIDTH], current_grid[HEIGHT][WIDTH];
    int i;

    test();

    fillGrid(previous_grid, INITSTATE);
    neillclrscrn();

    for (i = 0; i < GENERATIONS; i++) {
        evolution(previous_grid, current_grid);
        gridPrint(current_grid);
        /* copy current grid to become previous grid of next gen */
        gridCopy(previous_grid, current_grid);
    }

    return 0;
}

void test(void)
{
    int i, count;
    int his[2];
    state g1[HEIGHT][WIDTH] = {{0}};
    state g2[HEIGHT][WIDTH] = {{0}};
    state gridcpy[HEIGHT][WIDTH];
    state grid_tree1[HEIGHT][WIDTH] = {
        {empty, empty, empty},
        {empty, tree, fire},
        {empty, empty, empty}
    };
    state grid_tree2[HEIGHT][WIDTH] = {
        {empty, tree, tree, tree},
        {empty, tree, tree, empty},
        {tree, empty, empty, fire},
        {empty, fire, tree, tree}
    };
    displaytext d1, d2, d3;

    /* testing chance */
    for (i = 0; i < 1000; i++) {
        his[chance(10)]++;
    }
    /* output should be true 1 in 10 times so about 100 times */
    assert(his[1] > 90 && his[1] < 110);

    assert(gridMatch(g1, g1));
    assert(gridMatch(g1, grid_tree1) == 0);

    gridCopy(gridcpy, g1);
    assert(gridMatch(gridcpy, g1));

    assert(countState(grid_tree2, tree) == 8);
    assert(countState(grid_tree2, fire) == 2);

    assert(safeRead(grid_tree1, 1, 1) == tree);
    assert(safeRead(grid_tree1, -1, -5) == DEFAULT);

    d1 = stateToDisplay(empty);
    d2 = stateToDisplay(tree);
    d3 = stateToDisplay(fire);
    assert(d1.text == EMPTYCHAR);
    assert(d1.colour == black);
    assert(d2.text == TREECHAR);
    assert(d2.colour == green);
    assert(d3.text == FIRECHAR);
    assert(d3.colour == red);

    fillGrid(g1, empty);
    assert(countState(g1, empty) == (HEIGHT * WIDTH));

    assert(isFireNear(grid_tree1, 1, 1) == true);
    assert(isFireNear(grid_tree2, 1, 1) == false);

    assert(evoTree(grid_tree1, 1, 1) == fire);
    /* assuming unlikely to be hit by lightning */
    assert(evoTree(grid_tree2, 1, 1) == tree);

    /* test 1st evolution from empty board */
    evolution(g1, g2);
    count = countState(g2, tree);
    assert(count < 30);
    assert(countState(g2, fire) == 0);

}


void fillGrid(state grid[HEIGHT][WIDTH], state fill)
{
    int row, col;

    for (row = 0; row < HEIGHT; row++) {
        for (col = 0; col < WIDTH; col++){
            grid[row][col] = fill;
        }
    }
}

void evolution(state g1[HEIGHT][WIDTH], state g2[HEIGHT][WIDTH])
{
    state current, new;
    int row, col;

    for (row = 0; row < HEIGHT; row++) {
        for (col = 0; col < WIDTH; col++){
            current = g1[row][col];
            switch (current) {
                case fire:
                    /* fire always turns into empty in next gen */
                    new = empty;
                    break;
                case tree:
                    new = evoTree(g1, row, col);
                    break;
                case empty:
                    new = evoEmpty();
                    break;
                default:
                    fprintf(stderr,"Error, invalid state value\n");
                    exit(EXIT_FAILURE);
            }
            g2[row][col] = new;
        }
    }
}

state evoTree(state grid[HEIGHT][WIDTH], int row, int col)
{
    if (isFireNear(grid, row, col)) {
        return fire;
    }
    else if (chance(L)) {
        return fire;
    }

    return tree;
}

state evoEmpty(void)
{
    if (chance(G)) {
        return tree;
    }
    return empty;
}

bool isFireNear(state grid[HEIGHT][WIDTH], int row, int col)
{
    int y, x;
    state s;

    /* scanning 3 by 3 area centered on cell */
    for (y = row - 1; y <= row + 1; y++) {
        for (x = col - 1; x <= col + 1; x++){
            s = safeRead(grid, y, x);
            if(s == fire) {
                return true;
            }
        }
    }

    return false;
}

bool gridMatch(state g1[HEIGHT][WIDTH], state g2[HEIGHT][WIDTH])
{
    int row, col;

    for (row = 0; row < HEIGHT; row++) {
        for (col = 0; col < WIDTH; col++){
            if (g1[row][col] != g2[row][col]) {
                return false;
            }
        }
    }
    return true;
}

void gridCopy(state g1[HEIGHT][WIDTH], state g2[HEIGHT][WIDTH])
{
    int row, col;

    for (row = 0; row < HEIGHT; row++) {
        for (col = 0; col < WIDTH; col++){
            g1[row][col] = g2[row][col];
        }
    }
}

bool chance(int div)
{
    int n;

    n = rand() % div; /* n would be between 0 - div */
    if(n == 0) {
        return true;
    }
    return false;
}

displaytext stateToDisplay(state s)
{
    displaytext d;
    switch(s) {
        case empty:
            d.text = EMPTYCHAR;
            d.colour = black;
            break;
        case tree:
            d.text = TREECHAR;
            d.colour = green;
            break;
        case fire:
            d.text = FIRECHAR;
            d.colour = red;
            break;
        default:
            fprintf(stderr, "ERROR, invalid state value\n");
            exit(EXIT_FAILURE);
    }
    return d;
}

state safeRead(state g[HEIGHT][WIDTH], int row, int col)
{
    if (row < 0 || row >= HEIGHT || col < 0 || col >= WIDTH) {
        /* if out of bounds, return a default state e.g. empty */
        return DEFAULT;
    }

    return g[row][col];
}

void gridPrint(state g[HEIGHT][WIDTH])
{
    displaytext d;
    int row, col;
    state s;

    neillcursorhome();
    for (row = 0; row < HEIGHT; row++) {
        neillfgcol(white);
        for (col = 0; col < WIDTH; col++){
            s = g[row][col];
            d = stateToDisplay(s);
            neillfgcol(d.colour);
            printf("%c", d.text);
        }
        printf("\n");
    }
    printf("\n");
    neillfgcol(white);
    neillbusywait(WAITTIME);
}

int countState(state g[HEIGHT][WIDTH], state s)
{
    int row, col, count = 0;

    for (row = 0; row < HEIGHT; row++) {
        for (col = 0; col < WIDTH; col++){
            if (g[row][col] == s) {
                count ++;
            }
        }
    }

    return count;
}
