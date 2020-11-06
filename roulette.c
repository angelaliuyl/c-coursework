#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAXNUMONWHEEL 35
#define INITCASH 50.0
#define BETMULT 35
#define EVEN 2


typedef double money;
typedef enum bettype {odd_even, numbers} bettype;
typedef enum oetype {even, odd} oetype;


void test(void);
int spinwheel(void);
bettype whichbet(void);
money setbet(money p);
money oe_mode(money bet);
money num_mode(money bet);
oetype oddoreven(int i);

int main(void)
{

    money pot = INITCASH, bet = 0;
    bettype b;
    /*char c;*/

    test();

    while (pot > 0) {
        printf("Your pot is currently %.2f.\n", pot);

        b = whichbet();
        bet = setbet(pot);

        switch(b) {
            case odd_even :
                pot += oe_mode(bet);
                break;
            case numbers :
                pot += num_mode(bet);
                break;
            default :
                printf("Invalid bet type.\n");
        }

        /* wanted to write some code to give user option to terminate program
        without using ctrl-c but it wouldn't work for some reason.

        do {
            printf("Keep playing? y/n : ");
        } while (scanf("%c", &c) != 1 && c != 'y' && c != 'n');
        if (c == 'n') {
            return 0;
        }*/
    }

    printf("You've ran out of money in your pot.\n");
    return 0;
}

void test(void)
{
    int i, n;
    for(i = 0; i < 100000; i++) {
        n = spinwheel();
        assert((n >= 0) && (n <= MAXNUMONWHEEL));
    }
}

int spinwheel(void)
{
    return rand() % (MAXNUMONWHEEL + 1);
}

bettype whichbet(void)
{
    int inp;
    bettype b;

    do {
        printf("Please type 0 for Odd/Even bet type and 1 for Numbers bet type : ");
    } while ((scanf("%d", &inp)) != 1 && inp != 0 && inp != 1);
    /* keep asking until input is an int of 0 or 1*/

    /*converting scanf int to bettype*/
    switch(inp) {
        case 0 :
            b = odd_even;
            printf("Odd/Even mode chosen.\n");
            return b;
        case 1 :
            b = numbers;
            printf("Numbers mode chosen.\n");
            return b;
        default :
            printf("Invalid bet type.\n");
            exit(1);
    }

}

money setbet(money p)
{
    money bet;
    do {
        printf("You have %.2f in your pot. ", p);
        printf("Please enter amount to bet: ");
    } while ((scanf("%lf", &bet)) != 1 && bet < p);
    /* keep asking until input is a double within the pot amount */

    return bet;
}

money oe_mode(money bet)
{
    oetype guess;
    int inp, spin;
    money diff;
    /* Keep asking user for an input until correct input. */
    do {
        printf("Please enter 0 for even and 1 for odd : ");
    } while (((scanf("%d", &inp)) != 1) && inp != 0 && inp != 1);
    /* keep asking for input until input is an int of 0 or 1 */

    /* converting input to oetype */
    switch(inp) {
        case 0 :
            guess = even;
            printf("Even chosen.\n");
            break;
        case 1 :
            guess = odd;
            printf("Odd chosen.\n");
            break;
        default :
            printf("Invalid guess.\n");
            exit(1);
    }

    spin = spinwheel();
    printf("The spin was %d.\n", spin);

    /* Loses if spin is 0 or odd/even doesn't match*/
    if (spin == 0 || oddoreven(spin) != guess) {
        printf("You lost.\n");
        diff = -bet;
    }
    else {
        printf("You won.\n");
        diff = bet;
    }

    return diff;
}

money num_mode(money bet)
{
    int guess, spin;
    money diff;

    /* Keep asking user for an input until correct input. */
    do {
        printf("Please enter guess between 0 - %d : ", MAXNUMONWHEEL);
    } while ((scanf("%d", &guess)) != 1 && guess < 0 && guess > MAXNUMONWHEEL);

    printf("Your guess is %d.\n", guess);

    spin = spinwheel();
    printf("The spin was %d.\n", spin);

    if (spin == guess) {
        printf("You won.\n");
        diff = bet*(BETMULT - 1);
    }
    else {
        printf("You lost.\n");
        diff = -bet;
    }

    return diff;
}

oetype oddoreven(int i)
{
    if (i % 2 == 0) {
        return even;
    }
    else {
        return odd;
    }
}
