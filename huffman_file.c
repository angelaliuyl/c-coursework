/* NOT YET COMPLETED */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#define CHARNUM 128
#define BITNUM 8
#define STRSIZE 10000
/* #define SAMPLETXT "A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED" */
/* #define SAMPLETXT "BABBAGE" */


struct huff {
    char c;
    int count;
    char bin_rep[STRSIZE];
    int size;
    struct huff *next;
    struct huff *leftchild;
    struct huff *rightchild;
};
typedef struct huff Huff;

struct slist {
    Huff *start;
};
typedef struct slist Slist;


void read_cmd_line(int argc, char **argv, char *filename);
int MakeHist(char *filename, int *hist);
void PrintHist(int *hist, int size);
Huff *InitHuff(char c, int count);
void SortedHuff(int *hist, Slist *l);
void SortInsert(Slist *l, Huff *new);
void PrintHuff(Huff *start);
void PrintSlist(Slist *l);
void FreeHuff(Huff *start);
Huff *MakeTree(Slist *l);
char *PrintTree(Huff *head);
void HuffComp(Huff *head, char *str_o);
void PrintFinal(Huff *start);


int main(int argc, char **argv)
{
    int hist[CHARNUM] = {0}, size;
    Slist *l = calloc(1, sizeof(Slist));
    Huff *head;
    char str[STRSIZE], filename[STRSIZE];

    read_cmd_line(argc, argv, filename);
    size = MakeHist(filename, hist);

    /* printf("size of hist = %d\n", size); */
    /* PrintHist(hist, size); */

    SortedHuff(hist, l);
    /* PrintHuff(l->start); */

    head = MakeTree(l);
    /* PrintHuff(l->start); */

    /* printf("%s\n",PrintTree(head)); */
    str[0] = '\0';
    HuffComp(head, str);
    PrintFinal(l->start);
    printf("(Otherwise %d bits)\n", size);

    FreeHuff(l->start);
    free(l);
    return 0;

}

void read_cmd_line(int argc, char **argv, char *filename)
{
    if (argc == 2) {
        strcpy(filename, argv[1]);
    }
    else {
        fprintf(stderr, "Incorrect usage, try %s sherlock_holmes.txt", argv[0]);
        exit(EXIT_FAILURE);
    }
}


int MakeHist(char *filename, int *hist)
{
    int i = 0;
    FILE *fp;
    char c;
    if ((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    while((c = fgetc(fp)) != EOF) {
        hist[(int) c]++;
        i++;
    }

    fclose(fp);
    return i;
}

void PrintHist(int *hist, int size)
{
    int i;
    printf("size of this str = %d\n", size);
    for (i = 0; i < CHARNUM; i++) {
        if (hist[i]) {
            printf("'%c', count = %d\n", i, hist[i]);
        }
    }
}

Huff *InitHuff(char c, int count)
{
    Huff *p;
    p = calloc(1, sizeof(Huff));
    p->c = c;
    p->count = count;

    return p;
}

void SortedHuff(int *hist, Slist *l)
{
    int i;
    Huff *new;
    for (i = 0; i < CHARNUM; i++) {
        if (hist[i]) {
            /* printf("current char = '%c', count = %d\n", i, hist[i]); */
            new = InitHuff((char) i, hist[i]);
            SortInsert(l, new);
            /* PrintHuff(l->start); */
        }
    }
}

void SortInsert(Slist *l, Huff *new)
{
    Huff *start = l->start, *current, *previous;

    if (start == NULL) {
        start = new;
        /* printf("allocated first node\n"); */
    }
    else if (new->count < start->count) {
        new->next = start;
        start = new;
        /* printf("allocated node at front\n"); */
    }
    else {
        current = start->next;
        previous = start;
        while(current != NULL && current->count <= new->count) {
            previous = current;
            current = current->next;
        }
        previous->next = new;
        new->next = current;
        /* printf("allocated node behind '%c'\n", previous->c); */
    }
    l->start = start;
}

void PrintHuff(Huff *start)
{
    while(start != NULL)
    {
        printf("'%c'  = %d | ", start->c, start->count);
        start = start->next;
    }
    putchar('\n');
}

void PrintSlist(Slist *l)
{
    Huff *start = l->start;
    PrintHuff(start);
}

void FreeHuff(Huff *start)
{
    Huff *tmp;
    while(start != NULL)
    {
        tmp = start;
        start = start->next;
        free(tmp);
    }
}

Huff *MakeTree(Slist *l)
{
    Huff *new, *current = NULL, *leftchild, *rightchild;
    int c1, c2;
    do {
        if (current == NULL) {
            current = l->start;
        }
        else {
            current = current->next;
        }
        /* get left child */
        leftchild = current;
        c1 = current->count;
        current = current->next;
        /* get right child */
        rightchild = current;
        c2 = current->count;
        new = InitHuff('\0', c1 + c2);
        /* printf("new node with childs %d, %d\n", c1, c2); */
        new->leftchild = leftchild;
        new->rightchild = rightchild;
        SortInsert(l, new);
    } while (current->next->next != NULL);

    return new;
}

char *PrintTree(Huff *head)
{
    char *str;
    assert((str = calloc(STRSIZE, sizeof(char))) != NULL);
    if(head == NULL){
        strcpy(str, "*");
        return str;
    }
    sprintf(str, "%d (%s) (%s)", head->count, PrintTree(head->leftchild), PrintTree(head->rightchild));
    return str;
}

void HuffComp(Huff *head, char *str_o)
{
    char str[STRSIZE], str_add[STRSIZE];

    if (head->leftchild == NULL && head->rightchild == NULL) {
        strcpy(head->bin_rep, str_o);
        return;
    }

    strcpy(str, str_o);
    strcpy(str_add, "0");
    strcat(str, str_add);
    HuffComp(head->leftchild, str);

    strcpy(str, str_o);
    strcpy(str_add, "1");
    strcat(str, str_add);
    HuffComp(head->rightchild, str);
}

void PrintFinal(Huff *start)
{
    int count = 0, i;
    Huff *current;
    char c;
    for (i = 1; i < CHARNUM; i++) {
        c = (char) i;
        current = start;
        while(current != NULL)
        {
            /* printf("check = %c, current = %c\n", c, current->c); */
            if (current->c == c) {
                printf("'%c' : %20s (%3d * %3d) \n", current->c, current->bin_rep, (int) strlen(current->bin_rep), current->count);
                count += (int) strlen(current->bin_rep) * current->count;
            }
            current = current->next;
        }
    }

    printf("%d bytes\n", count / BITNUM);
}
