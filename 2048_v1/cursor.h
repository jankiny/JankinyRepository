/* cursor handle need to include: */

/* principles: First, we create a Canvas(define a rectangular space),   
               it was full of ' '(space), and surranded by black lines. 
               Then, we can draw the Canvas with character and cursor. */

/* As for the direction, x is pointing to right, and y is pointing to down */

#include <stdio.h>
#include <stdlib.h>
#ifndef __CURSOR_H__
#define __CURSOR_H__
#define SPACE ' '
int x, y;    //cursors
int range_x = 0;
int range_y = 0;
char **cvas;
int mode = 2; // 1(use): hide detials; 2(debug):show detials

// create a Canvas
char **canvas(int a, int b) {
    int i, j;

    cvas = (char **)malloc((a+2)*sizeof(char *));
    for(i = 0; i < a+2; i++)
        cvas[i] = (char *)malloc((b+2)*sizeof(char));

    range_x = a+1;
    range_y = b+1;
    for(i = 0; i <= a+1; i++) {
        for(j = 0; j <= b+1; j++) {
            x = i, y = j;
            if(i == 0 || i == a+1)
                cvas[i][j] = '-';
            else if(j == 0 || j == b+1)
                cvas[i][j] = '|';
            else
                cvas[i][j] = ' ';
        }
    }
    return cvas;
}

void addch(char ch) {
    extern char **cvas;

    cvas[x][y] = ch;
}
/* //Can't work
void delch() {
    extern char **cvas;

    cvas[x][y] == SPACE;
}
*/
void cursor_move(int a, int b) {
    if(a == 0 || b == 0 || a >= range_x || b >= range_y) {
        printf("Out of range! \nexit \n");
        exit(1);
    }
    x = a;
    y = b;
}

void char_move(int a, int b) {
    char temp;

    if(a == 0 || b == 0 || a >= range_x || b >= range_y) {
        printf("Out of range! \nexit \n");
        exit(1);
    }
    temp = cvas[x][y];
    cvas[x][y] = SPACE;
    x = a;
    y = b;
    cvas[x][y] = temp;
}

void refresh() {
    int i, j;

    system("reset");
//    printf("\033[2J"); // screen output control: clear screen
    for(i = 0; i <= range_x; i++) {
        for(j = 0; j <= range_y; j++) {
             printf("%c", cvas[i][j]);
        }
        putchar('\n');
    }
}

void changeMode(int n) {
    if(n >= 1 && n <= 2) {
        mode = n;
    }
    else
        printf("Only 1-2 modes exist!\n");
}
#endif
