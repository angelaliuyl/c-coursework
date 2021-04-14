/*
    TODO
    V dealing with undefined variables when allowing negative values
    V not used fixed size arrays for wds and polstack
        - implement queue and array from ADTs?
    V compiling into parser and interp from same turtle.c code
    V testing
    * think of extensions: add more to grammar seems straightforward:
        - add conditional statements such as while or if?
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>

#include "ADTs/General/bool.h"
#include "ADTs/General/general.h"

#ifdef INTERPRETER
#ifndef TEST
#include "neillsdl2.h"
#endif /* TEST */
#endif /* INTERPRETER */

#define MAXNUMTOKENS 100
#define MAXTOKENSIZE 10
#define strsame(A, B) (strcmp(A, B) == 0)
#define ALPHA 26
#define STACKSIZE 100
#define STRSIZE 50
#define SCALEFACTOR 2

#ifdef INTERPRETER /* marcos for SDL */
#define deg2rad(deg) (deg * M_PI / 180)
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif /* M_PI */
#define FLOATDIFF 0.00001
#define fltsame(A, B) (fabs(A - B) < FLOATDIFF)
#ifndef TEST
#define MILLISECONDDELAY 20
#define SDLWHITE 255
#endif /* TEST */
#endif /* INTERPRETER */

#ifndef DEBUG
#define DEBUG false
#endif /* DEBUG */

struct vari {
    bool set; /* checks whether the variable has been set */
    float value; /* holds the value of the variable */
};
typedef struct vari variable;

#ifdef INTERPRETER
struct coord {
    float x;
    float y;
    float angle;
};
typedef struct coord coordinate;

typedef enum lop {and, or} lop;
typedef enum relat {eq, ne, gt, ge, lt, le} relat;
#endif /* INTERPRETER */

struct prog{
    char **wds; /* 2D array of "words" read from the file */
    int wdsize; /* current capacity of the wds array */
    int cw; /* current word being read - i.e. the program counter */
    variable vars[ALPHA]; /* stores value of all variables A-Z */
    char operand; /* current variable in use */
    float num; /* current number in use */


    #ifdef INTERPRETER /* variables for interpreter */
    bool conds[2];
    int condnum;
    lop logicop;
    relat relationship;

    float *polstack; /* resizable stack for doing polish notation calc */
    int psi; /* index for top of polish stack */
    int pssize; /* current capacity of the stack */
    coordinate curcoord;
    #ifndef TEST /* variables for sdl */
    SDL_Simplewin sw;
    #endif /* TEST */
    #endif /* INTERPRETER */
};
typedef struct prog Program;

void read_file(Program *p, int argc, char **argv);
void init_prog(Program *p);
void term_prog(Program *p);

void Main(Program *p);
void Instruclist(Program *p);
void Instruction(Program *p);
void Fd(Program *p);
void Lt(Program *p);
void Rt(Program *p);
void Do(Program *p);
void If(Program *p);
void Cond(Program *p);
void Logexp(Program *p);
void Lop(Program *p);
void Relat(Program *p);
void Var(Program *p);
void Varnum(Program *p);
void Number(Program *p);
void Set(Program *p);
void Polish(Program *p);
void Op(Program *p);

#ifdef INTERPRETER /* functions for interpreter */
void test(void);
coordinate Change(Program *p, float length);
void Movement(Program *p, float length);
#ifndef TEST
void term_sdl(Program *p);
void DrawLine(Program *p, float length);
#endif /* TEST */
#endif /* INTERPRETER */

void Error(const char *phrase);

int main(int argc, char **argv)
{
    Program prog;
    #ifdef INTERPRETER
    #ifdef TEST
    test();
    #endif /* TEST */
    #endif /* INTERPRETER */

    read_file(&prog, argc, argv);
    Main(&prog);
    printf("Parsed OK\n");

    term_prog(&prog);
    return 0;
}

void read_file(Program *p, int argc, char **argv)
{
    int i;
    char filename[STRSIZE], errmsg[STRSIZE];
    FILE *fp;

    if (argc != 2) {
        sprintf(errmsg, "Incorrect usage, try %s simp.ttl\n", argv[0]);
        Error(errmsg);
    }
    else {
        strcpy(filename, argv[1]);
    }

    init_prog(p);

    fp = nfopen(filename, "r");
    i = 0;
    while(fscanf(fp, "%s", p->wds[i++]) == 1){
        /* if array is filled, realloc the array according to scale factor */
        if (i >= p->wdsize) {
            p->wds = (char **) n2drecalloc((void **)p->wds, p->wdsize,
                                           p->wdsize * SCALEFACTOR,
                                           MAXTOKENSIZE, MAXTOKENSIZE,
                                           sizeof(char));
            p->wdsize *= SCALEFACTOR;
        }
    }
    fclose(fp);
}

void init_prog(Program *p)
{
    int i;

    #ifdef INTERPRETER
    #ifndef TEST
    SDL_Simplewin sw;
    #endif /* TEST */
    #endif /* INTERPRETER */

    p->cw = 0;
    p->wds = (char **) n2dcalloc(MAXNUMTOKENS, MAXTOKENSIZE, sizeof(char));
    for(i = 0; i<ALPHA; i++) {
        p->vars[i].set = false;
        p->vars[i].value = 0;
    }
    p->wdsize = MAXNUMTOKENS;
    p->num = -1;

    #ifdef INTERPRETER
    p->polstack = ncalloc(STACKSIZE, sizeof(float));
    p->psi = -1;
    p->pssize = STACKSIZE;

    p->conds[0] = false;
    p->conds[1] = false;
    p->condnum = -1;

    #ifndef TEST
    /* set starting point to center of screen */
    p->curcoord.angle = 0;
    p->curcoord.x = WWIDTH / 2;
    p->curcoord.y = WHEIGHT / 2;
    Neill_SDL_Init(&sw);
    p->sw = sw;
    #else
    /* during testing, just set origin to (0, 0)*/
    p->curcoord.angle = 0;
    p->curcoord.x = 0;
    p->curcoord.y = 0;
    #endif /* TEST */
    #endif /* INTERPRETER */
}

void term_prog(Program *p)
{
    #ifdef INTERPRETER
    #ifndef TEST
    term_sdl(p);
    #endif /* TEST */
    free(p->polstack);
    #endif /* INTERPRETER */
    n2dfree((void **)p->wds, p->wdsize);
}

#ifdef INTERPRETER
#ifndef TEST
void term_sdl(Program *p)
{
    /* adapted from Neill's blocks.c code */
    SDL_Simplewin sw = p->sw;
    do{

       SDL_Delay(MILLISECONDDELAY);

       /* Has anyone pressed ESC or killed the SDL window ?
          Must be called frequently - it's the only way of escaping  */
       Neill_SDL_Events(&sw);

    }while(!sw.finished);

    /* Clear up graphics subsystems */
    SDL_Quit();
    atexit(SDL_Quit);
}
#endif /* TEST */
#endif /* INTERPRETER */

void Main(Program *p)
{
    if (DEBUG) printf("Main: %d\n", p->cw);
    if(!strsame(p->wds[p->cw], "{")) {
        Error("No '{' at beginning of file\n");
    }
    p->cw++;
    Instruclist(p);
}

void Instruclist(Program *p)
{
    if (DEBUG) printf("Instruclist: %d\n", p->cw);
    if(strsame(p->wds[p->cw], "}")) {
        return;
    }
    if(strsame(p->wds[p->cw], "\0")) {
        Error("Reached EOF without '}'\n");
    }
    Instruction(p);
    p->cw++;
    Instruclist(p);
}

void Instruction(Program *p)
{
    char* str = p->wds[p->cw];

    if (DEBUG) printf("Instruction: %d\n", p->cw);
    if(strsame(str, "FD")) {
        Fd(p);
    }
    else if (strsame(str, "LT")) {
        Lt(p);
    }
    else if (strsame(str, "RT")) {
        Rt(p);
    }
    else if (strsame(str, "DO")) {
        Do(p);
    }
    else if (strsame(str, "SET")) {
        Set(p);
    }
    else if (strsame(str, "IF")) {
        If(p);
    }
    else {
        if (DEBUG) printf("Instruction selected = %s ", str);
        Error("Instruction not recognised.\n");
    }
}

void Fd(Program *p)
{
    if (DEBUG) printf("Fd: %d\n", p->cw);

    if (!strsame(p->wds[p->cw], "FD")) {
        /* this shouldn't happen, but just in case */
        Error("Instruction not recognised?\n");
    }
    p->cw++;
    Varnum(p);

    #ifdef INTERPRETER

    #ifndef TEST
    DrawLine(p, p->num);
    #endif /* TEST */
    if (DEBUG) printf("FD %.2f\n", p->num);
    Movement(p, p->num);
    #endif /* INTERPRETER */

}

void Lt(Program *p)
{
    if (DEBUG) printf("Lt: %d\n", p->cw);

    if (!strsame(p->wds[p->cw], "LT")) {
        Error("Instruction not recognised?\n");
    }
    p->cw++;
    Varnum(p);
    #ifdef INTERPRETER
    if (DEBUG) printf("LT %.2f\n", p->num);
    p->curcoord.angle -= p->num;
    #endif /* INTERPRETER */

}

void Rt(Program *p)
{
    if (DEBUG) printf("Rt: %d\n", p->cw);

    if (!strsame(p->wds[p->cw], "RT")) {
        Error("Instruction not recognised?\n");
    }
    p->cw++;
    Varnum(p);

    #ifdef INTERPRETER
    if (DEBUG) printf("RT %.2f\n", p->num);
    p->curcoord.angle += p->num;
    #endif /* INTERPRETER */

}

void Do(Program *p)
{
    #ifdef INTERPRETER
    char var;
    int init, max, loopstart;
    #endif /* INTERPRETER */

    if (DEBUG) printf("Do: %d\n", p->cw);

    if (!strsame(p->wds[p->cw], "DO")) {
        Error("DO instruction: incorrect format, expecting DO\n");
    }
    p->cw++;
    Var(p);

    #ifdef INTERPRETER
    var = p->operand;
    #endif /* INTERPRETER */

    p->cw++;
    if (!strsame(p->wds[p->cw], "FROM")) {
        Error("DO instruction: incorrect format, expecting FROM\n");
    }
    p->cw++;
    Varnum(p);

    #ifdef INTERPRETER
    init = p->num;
    #endif /* INTERPRETER */

    p->cw++;
    if (!strsame(p->wds[p->cw], "TO")) {
        Error("DO instruction: incorrect format, expecting TO\n");
    }
    p->cw++;
    Varnum(p);

    #ifdef INTERPRETER
    max = p->num;
    if (max < init) {
        Error("DO instruction: variable goes from higher number to lower number\n");
    }
    #endif /* INTERPRETER */

    p->cw++;
    if (!strsame(p->wds[p->cw], "{")) {
        Error("DO instruction: incorrect format, expecting {\n");
    }
    p->cw++;

    #ifdef INTERPRETER
    loopstart = p->cw;
    p->vars[var - 'A'].value = init;
    p->vars[var - 'A'].set = true;
    if (DEBUG) printf("var = %c\n", var);
    while(p->vars[var - 'A'].value <= max) {
        if (DEBUG) printf("In loop %d\n", (int) p->vars[var - 'A'].value);
        p->cw = loopstart; /* goes to start of loop */
        Instruclist(p);
        p->vars[var - 'A'].value++;
    }
    #else
    Instruclist(p);
    #endif /* INTERPRETER */
}

void If(Program *p)
{
    if (DEBUG) printf("If: %d\n", p->cw);

    if (!strsame(p->wds[p->cw], "IF")) {
        Error("IF instruction: incorrect format, expecting IF\n");
    }
    p->cw++;

    Cond(p);

    p->cw++;
    if (!strsame(p->wds[p->cw], "{")) {
        if (DEBUG) printf("wds = %s", p->wds[p->cw]);
        Error("IF instruction: incorrect format, expecting {\n");
    }
    p->cw++;

    #ifdef INTERPRETER
    if (p->condnum != 0) {
        Error("IF instruction: not enough logical operators\n");
    }
    p->condnum--;
    if (p->conds[0]) {
        Instruclist(p);
    }
    else {
        while (!strsame(p->wds[p->cw], "}")) {
            p->cw++;
        }
    }
    #else
    Instruclist(p);
    #endif /* INTERPRETER */
}

void Cond(Program *p)
{
    if (DEBUG) printf("Cond: %d\n", p->cw);
    Logexp(p);

    #ifdef INTERPRETER
    if (p->condnum == 1) {
        /* there are two conditionals */
        if (p->logicop == and) {
            p->conds[0] = p->conds[0] && p->conds[1];
        }
        else if (p->logicop == or) {
            p->conds[0] = p->conds[0] || p->conds[1];
        }
        else {
            Error("Something went wrong while evaluating cond\n");
        }
        p->condnum--;
    }
    #endif /* INTERPRETER */

    if (strsame(p->wds[p->cw], ";")) {
        return;
    }

    Lop(p);
    p->cw++;

    Cond(p);
}

void Logexp(Program *p)
{
    float a, b;
    bool cond;
    if (DEBUG) printf("Logexp: %d\n", p->cw);
    if (strsame(p->wds[p->cw], "!")) {
        p->cw++;
        Logexp(p);

        #ifdef INTERPRETER
        if (p->conds[p->condnum]) {
            p->conds[p->condnum] = false;
        }
        else {
            p->conds[p->condnum] = true;
        }
        #endif /* INTERPRETER */

        return;
    }

    Varnum(p);
    p->cw++;
    #ifdef INTERPRETER
    a = p->num;
    #endif /* INTERPRETER */

    Relat(p);
    p->cw++;

    Varnum(p);
    p->cw++;
    #ifdef INTERPRETER
    b = p->num;
    #endif /* INTERPRETER */

    #ifdef INTERPRETER
    switch (p->relationship) {
        case eq:
            cond = fltsame(a, b);
            break;
        case ne:
            cond = !fltsame(a, b);
            break;
        case gt:
            cond = a > b;
            break;
        case ge:
            cond = (a > b) || fltsame(a, b);
            break;
        case lt:
            cond = (a < b);
            break;
        case le:
            cond = (a < b) || fltsame(a, b);
            break;
        default:
            Error("something went wrong in evalutation logexp\n");
    }
    p->conds[++p->condnum] = cond;
    #endif /* INTERPRETER */

}

void Lop(Program *p)
{
    if (DEBUG) printf("Lop: %d\n", p->cw);

    if (strsame(p->wds[p->cw], "&&")) {
        #ifdef INTERPRETER
        p->logicop = and;
        #endif /* INTERPRETER */
    }
    else if (strsame(p->wds[p->cw], "||")) {
        #ifdef INTERPRETER
        p->logicop = or;
        #endif /* INTERPRETER */
    }
    else {
        Error("Logical operator not recognised\n");
    }
}

void Relat(Program *p)
{
    if (DEBUG) printf("Relat: %d\n", p->cw);

    if (strsame(p->wds[p->cw], "==")) {
        #ifdef INTERPRETER
        p->relationship = eq;
        #endif /* INTERPRETER */
    }
    else if (strsame(p->wds[p->cw], "!=")) {
        #ifdef INTERPRETER
        p->relationship = ne;
        #endif /* INTERPRETER */
    }
    else if (strsame(p->wds[p->cw], ">")) {
        #ifdef INTERPRETER
        p->relationship = gt;
        #endif /* INTERPRETER */
    }
    else if (strsame(p->wds[p->cw], ">=")) {
        #ifdef INTERPRETER
        p->relationship = ge;
        #endif /* INTERPRETER */
    }
    else if (strsame(p->wds[p->cw], "<")) {
        #ifdef INTERPRETER
        p->relationship = lt;
        #endif /* INTERPRETER */
    }
    else if (strsame(p->wds[p->cw], "<=")) {
        #ifdef INTERPRETER
        p->relationship = le;
        #endif /* INTERPRETER */
    }
    else {
        Error("Relationship not recognised\n");
    }
}


void Var(Program *p)
{
    char c;
    if (DEBUG) printf("Var: %d\n", p->cw);

    if (strlen(p->wds[p->cw]) != 1) {
        Error("Variable more than 1 char\n");
    }
    c = p->wds[p->cw][0];
    if (c < 'A' || c > 'Z') {
        Error("Variable not character A - Z\n");
    }

    #ifdef INTERPRETER
    p->operand = c;
    #endif /* INTERPRETER */
}

void Varnum(Program *p)
{
    char c;
    if (DEBUG) printf("Varnum: %d\n", p->cw);
    c = p->wds[p->cw][0];
    if (isdigit(c)) {
        Number(p);
    }
    else {
        Var(p);

        #ifdef INTERPRETER
        if (!p->vars[c-'A'].set) {
            Error("Variable is undefined when referenced\n");
        }
        p->num = p->vars[c - 'A'].value;
        #endif /* INTERPRETER */
    }
}

void Number(Program *p)
{
    int i = 0;
    char c;
    if (DEBUG) printf("Number: %d\n", p->cw);

    while((c = p->wds[p->cw][i]) != '\0') {
        if (!isdigit(c) && c != '.' && c != '-') {
            Error("Incorrect number format in instruction\n");
        }
        i++;
    }

    #ifdef INTERPRETER
    p->num = atof(p->wds[p->cw]);
    #endif /* INTERPRETER */
}

void Set(Program *p)
{
    #ifdef INTERPRETER
    char var;
    #endif /* INTERPRETER */

    if (DEBUG) printf("Set: %d\n", p->cw);

    if (!strsame(p->wds[p->cw], "SET")) {
        Error("SET instruction : expecting SET\n");
    }
    p->cw++;
    Var(p);

    #ifdef INTERPRETER
    var = p->operand;
    #endif /* INTERPRETER */

    p->cw++;
    if (!strsame(p->wds[p->cw], ":=")) {
        Error("SET instruction : expecting :=\n");
    }
    p->cw++;
    Polish(p);

    #ifdef INTERPRETER
    p->vars[var - 'A'].set = true;
    p->vars[var - 'A'].value = p->num;
    if (DEBUG) printf("Set %c as %f\n", var, p->num);
    #endif /* INTERPRETER */
}

void Polish(Program *p)
{
    char c;
    if (DEBUG) printf("Polish: %d\n", p->cw);

    if (strsame(p->wds[p->cw], ";")) {
        /* end of polish expression reached */
        #ifdef INTERPRETER
        /* if there isn't only 1 number left on the stack, something's wrong */
        if (p->psi != 0) {
            Error("Incorrect Polish notation, numbers left on stack\n");
        }
        /* pop final number off stack to p->num */
        p->num = p->polstack[p->psi--];
        if (DEBUG) printf("Polish returns %f\n", p->num);
        #endif /* INTERPRETER */

        return;
    }
    c = p->wds[p->cw][0]; /* get first char of word to check */
    if (isalnum(c)) {
        Varnum(p);

        #ifdef INTERPRETER
        /* pushing number or value of variable onto stack */
        p->polstack[++p->psi] = p->num;
        /* if items in stack reach size capacity, realloc more space */
        if(p->psi >= p->pssize) {
            p->polstack = nrecalloc(p->polstack, p->pssize,
                                    p->pssize * SCALEFACTOR);
            p->pssize *= SCALEFACTOR;
        }
        #endif /* INTERPRETER */

        p->cw++;
        Polish(p);
    }
    else {
        Op(p);
        p->cw++;
        Polish(p);
    }
}

void Op(Program *p)
{
    char c;

    #ifdef INTERPRETER
    float a, b;
    #endif /* INTERPRETER */

    if (DEBUG) printf("Op: %d\n", p->cw);

    if (strlen(p->wds[p->cw]) != 1) {
        Error("Operator more than 1 char\n");
    }
    c = p->wds[p->cw][0];

    #ifdef INTERPRETER
    if (p->psi < 1) {
        /* if there are less than two numbers (index is lower than 1) */
        Error("Incorrect Polish notation, not enough numbers in stack\n");
    }
    /* popping off two items from the stack */
    b = p->polstack[p->psi--];
    a = p->polstack[p->psi--];
    switch(c) {
        case '+':
            p->polstack[++p->psi] = a + b;
            if (DEBUG) printf("operator = %f\n", a+b);
            return;
        case '-':
            p->polstack[++p->psi] = a - b;
            if (DEBUG) printf("operator = %f\n", a-b);
            return;
        case '*':
            p->polstack[++p->psi] = a * b;
            if (DEBUG) printf("operator = %f\n", a*b);
            return;
        case '/':
            p->polstack[++p->psi] = a / b;
            if (DEBUG) printf("operator = %f\n", a/b);
            return;
        default:
            Error("Unrecognised operator\n");
    }
    #else
    switch(c) {
        case '+':
            return;
        case '-':
            return;
        case '*':
            return;
        case '/':
            return;
        default:
            Error("Unrecognised operator\n");
    }
    #endif /* INTERPRETER */
}

void Error(const char* phrase)
{
    fprintf(stderr, "%s", phrase);
    exit(2);
}

#ifdef INTERPRETER
coordinate Change(Program *p, float length)
{
    coordinate change;

    /* consider angle 0 as pointing north */
    change.x = length * sin(deg2rad(p->curcoord.angle));
    change.y = -length * cos(deg2rad(p->curcoord.angle));
    change.angle = 0;

    return change;
}

void Movement(Program *p, float length)
{
    coordinate change;

    change = Change(p, length);

    p->curcoord.x += change.x;
    p->curcoord.y += change.y;
}

#ifndef TEST
void DrawLine(Program *p, float length)
{
    int x0, y0, x1, y1;
    SDL_Simplewin sw = p->sw;
    coordinate change;

    Neill_SDL_SetDrawColour(&sw, SDLWHITE, SDLWHITE, SDLWHITE);

    x0 = (int) p->curcoord.x;
    y0 = (int) p->curcoord.y;

    change = Change(p, length);

    x1 = (int) (p->curcoord.x + change.x);
    y1 = (int) (p->curcoord.y + change.y);
    if (DEBUG) printf("angle = %f, x0 = %d, y0 = %d, x1 = %d, y1 = %d\n",
              p->curcoord.angle, x0, y0, x1, y1);

    SDL_RenderDrawLine(sw.renderer, x0, y0, x1, y1);
    Neill_SDL_UpdateScreen(&sw);

}
#endif /* TEST */
#endif /* INTERPRETER */

#ifdef INTERPRETER
void test(void)
{
    int i;
    char filename[] = "asserttest.ttl";
    FILE *fp;
    Program prog, *p;

    assert(fltsame(0.0000, 0));
    assert(fltsame(0.001, 0.001));
    assert(!fltsame(0.001, 0));

    p = &prog;
    init_prog(p);

    fp = nfopen(filename, "r");
    i = 0;
    while(fscanf(fp, "%s", p->wds[i++]) == 1){
        /* if array is filled, realloc the array according to scale factor */
        if (i == p->wdsize) {
            p->wds = (char **) n2drecalloc((void **)p->wds, p->wdsize,
                                           p->wdsize * SCALEFACTOR,
                                           MAXTOKENSIZE, MAXTOKENSIZE,
                                           sizeof(char));
            p->wdsize *= SCALEFACTOR;
        }
    }
    fclose(fp);

    /* First part of code */
    Main(p);
    assert(fltsame(p->curcoord.x, 50));
    assert(fltsame(p->curcoord.y, 50));
    assert(fltsame(p->curcoord.angle, 180));

    /* Second part of code */
    p->cw++;
    p->curcoord.x = 0;
    p->curcoord.y = 0;
    p->curcoord.angle = 0;
    Main(p);
    assert(fltsame(p->curcoord.x, -2));
    assert(fltsame(p->curcoord.y, -3));
    assert(fltsame(p->curcoord.angle, 450));

    /* Third part of code */
    p->cw++;
    p->curcoord.x = 0;
    p->curcoord.y = 0;
    p->curcoord.angle = 0;
    Main(p);
    assert(fltsame(p->curcoord.x, 0));
    assert(fltsame(p->curcoord.y, 0));
    assert(fltsame(p->curcoord.angle, 2520));

    /* Fourth part of code */
    p->cw++;
    p->curcoord.x = 0;
    p->curcoord.y = 0;
    p->curcoord.angle = 0;
    Main(p);
    assert(fltsame(p->num, 5));
    assert(p->vars[0].set);
    assert(fltsame(p->vars[0].value, 5));

    /* Fifth part of code */
    p->cw++;
    p->curcoord.x = 0;
    p->curcoord.y = 0;
    p->curcoord.angle = 0;
    Main(p);
    assert(fltsame(p->num, 60));
    assert(p->vars[23].set);
    assert(fltsame(p->vars[23].value, 60));

    /* Sixth part of code */
    p->cw++;
    p->curcoord.x = 0;
    p->curcoord.y = 0;
    p->curcoord.angle = 0;
    Main(p);
    assert(p->operand == 'M');
    assert(fltsame(p->num, 10));
    assert(p->vars[12].set);
    assert(fltsame(p->vars[12].value, 10));

    term_prog(p);

}
#endif /* INTERPRETER */
