#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define MAXCOORD 1000
#define DEFAULT false
#define WIDTH 150
#define HEIGHT 90

struct coordinate {
    int x;
    int y;
};
typedef struct coordinate coordinate;
typedef enum bool {false, true} bool;

/* clean up coordinates for use in array */
void clean_active(coordinate *active, int len);
/* given array of active cell positions, initialize grid */
void initialize_grid(coordinate *active, int len, bool grid[HEIGHT][WIDTH]);
/* given 2 grids, turn grid 2 into the next gen of grid 1 */
void next_gen(bool grid1[HEIGHT][WIDTH], bool grid2[HEIGHT][WIDTH]);
/* given a grid and coordinates, produce evolution of coordinates */
bool evolve(bool grid[HEIGHT][WIDTH], int r, int c);
/* count number of inhabited cells around coordinates */
int count_inhabited(bool grid[HEIGHT][WIDTH], int r, int c);
/* performs a reading from grid taking into account out of bounds cases */
bool safe_read(bool grid[HEIGHT][WIDTH], int r, int c);
/* compares two grids to see if they match */
bool isgridmatch(bool grid1[HEIGHT][WIDTH], bool grid2[HEIGHT][WIDTH]);
/* copies content of grid2 into grid1 */
void gridcpy(bool grid1[HEIGHT][WIDTH], bool grid2[HEIGHT][WIDTH]);
/* print a grid */
void printgrid(bool grid[HEIGHT][WIDTH]);
void test(void);

int main(int argc, char **argv)
{
    int iterations, i, scan, x, y, len;
    char *filename, c;
    FILE *fp;
    coordinate active[MAXCOORD];
    bool grid1[HEIGHT][WIDTH] = {{0}}, grid2[HEIGHT][WIDTH] = {{0}};

    test();

    if (argc == 3) {
        filename = argv[1];
        iterations = atoi(argv[2]);

        /* opening file */
        if ((fp = fopen(filename, "r")) == NULL) {
            printf("Cannot open file\n");
            exit(EXIT_FAILURE);
        }

        /* read stuff from file */
        /* ignore first line */

        do {
            c = fgetc(fp);
            /*
            printf("%c", c);
            */
        } while (c != EOF && c != '\n');
        printf("\n");

        /* read everything into array of string */
        i = 0;
        do {
            if (i < MAXCOORD) {
                scan = fscanf(fp, "%d %d", &x, &y);
                if (scan != EOF) {
                    active[i].x = x;
                    active[i].y = y;
                    /*
                    printf("i = %d\n", i);
                    printf("x = %d, y = %d\n", active[i].x, active[i].y);
                    */
                    i++;
                }
            }
            else {
                printf("Max num of coordinates reached, error.\n");
                exit(EXIT_FAILURE);
            }
        } while (scan != EOF);
        fclose(fp);

        /* record number of coordinates scanned */
        len = i;
        clean_active(active, len);

        /* initialize grid according to active cells */

        initialize_grid(active, len, grid1);
        printgrid(grid1);

        /* iterate */
        for (i = 0; i < iterations; i++) {
            next_gen(grid1, grid2);
            printgrid(grid2);
        }

    }
    else {
        printf("ERROR: Incorrect usage, try e.g. %s filename.lif 100\n", argv[0]);
        return 1;
    }
    return 0;
}

void test(void)
{
    coordinate active[3] = {
        {2, 1},
        {2, 2},
        {2, 3}
    };
    int len = sizeof(active) / sizeof(active[0]);
    bool grid1[HEIGHT][WIDTH] = {{0}}, grid2[HEIGHT][WIDTH] = {{0}};
    bool grid_ini[HEIGHT][WIDTH] = {
        {0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0}
    };
    bool grid_evo[HEIGHT][WIDTH] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };

    assert(isgridmatch(grid_ini, grid_ini));
    assert(evolve(grid_ini, 2, 1) == 1);
    assert(count_inhabited(grid_ini, 2, 2) == 2);
    assert(safe_read(grid_ini, 0, 0) == 0);
    assert(safe_read(grid_ini, -1, -1) == DEFAULT);

    initialize_grid(active, len, grid1);
    assert(isgridmatch(grid1, grid_ini));

    next_gen(grid1, grid2);
    assert(isgridmatch(grid2, grid_evo));

    next_gen(grid1, grid2);
    assert(isgridmatch(grid2, grid_ini));

}

void clean_active(coordinate *active, int len)
{
    int i, x_mid = WIDTH/2, y_mid = HEIGHT/2;
    for (i = 0; i < len; i++) {
        active[i].x += x_mid;
        active[i].y += y_mid;
    }
}

void initialize_grid(coordinate *active, int len, bool grid[HEIGHT][WIDTH])
{
    int i, r, c;

    for (i = 0; i < len; i++) {
        r = active[i].y;
        c = active[i].x;
        grid[r][c] = 1;
    }
}

void next_gen(bool grid1[HEIGHT][WIDTH], bool grid2[HEIGHT][WIDTH])
{
    int row, col;

    for (row = 0; row < HEIGHT; row++) {
        for (col = 0; col < WIDTH; col++) {
            grid2[row][col] = evolve(grid1, row, col);
        }
    }

    gridcpy(grid1, grid2);
}

bool evolve(bool grid[HEIGHT][WIDTH], int r, int c)
{
    int neighbours;

    neighbours = count_inhabited(grid, r, c);
    if (safe_read(grid, r, c) == true) {
        if (neighbours == 2 || neighbours == 3) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        if (neighbours == 3) {
            return true;
        }
        else {
            return false;
        }
    }
}

int count_inhabited(bool grid[HEIGHT][WIDTH], int r, int c)
{
    int y, x, cnt = 0;

    /* looping through cells that are -1 to +1 of r and c */
    for (y = r-1; y <= r+1; y++) {
        for (x = c-1; x <= c+1; x++) {
            /* don't count current cell */
            if (y == r && x == c) {
                /* do nothing */
            }
            else {
                cnt += safe_read(grid, y, x);
            }
        }
    }

    return cnt;
}

bool safe_read(bool grid[HEIGHT][WIDTH], int r, int c)
{
    if (r < 0 || r >= HEIGHT || c < 0 || c >= WIDTH) {
        return DEFAULT;
    }
    else {
        return grid[r][c];
    }
}

bool isgridmatch(bool grid1[HEIGHT][WIDTH], bool grid2[HEIGHT][WIDTH])
{
    int row, col;

    for (row = 0; row < HEIGHT; row++) {
        for (col = 0; col < WIDTH; col++) {
            if (grid1[row][col] != grid2[row][col]) {
                return false;
            }
        }
    }

    return true;
}

void gridcpy(bool grid1[HEIGHT][WIDTH], bool grid2[HEIGHT][WIDTH])
{
    int row, col;

    for (row = 0; row < HEIGHT; row++) {
        for (col = 0; col < WIDTH; col++) {
            grid1[row][col] = grid2[row][col];
        }
    }
}

void printgrid(bool grid[HEIGHT][WIDTH])
{
    int row, col, val;

    for (row = 0; row < HEIGHT; row++) {
        printf("|");
        for (col = 0; col < WIDTH; col++) {
            val = grid[row][col];
            if(val) {
                printf("1");
            }
            else {
                printf(" ");
            }

        }
        printf("|\n");
    }
    printf("|");
    for (col = 0; col < WIDTH; col++) {
        printf("_");
    }
    printf("|\n\n");
}
