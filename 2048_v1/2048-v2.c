#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cursor.h"

char **cvas;    // Canvas
int mat[4][4];
//int a[4], b[4];    // save cooedinate
int emp[16];    // use to product number in empty position
int empty = 16;    // use to judge gameover

void init();    // initialization
void map_mat();    // map matrix to canvas
void mat_to_canvas(int x, int y, int a, int b);
char itoc(int n);
int randN(int m);    // return 4 or 2, 'm' mean probability of '4'
void judge_empty();    //judge empty position
void product_number();    // product number in empty position
void play();    // game main logic
//int line_empty(int r);    // whether empty
//void doubleN(int x, int y);    // add two same numbers 

int main() {
    init();
    refresh();
//    play();
}

// create a canvas, and draw line, product begining number
void init() {
    int i, j;
    int temp;

    // draw frame
    cvas = canvas(7, 15);    //need line x:2 4 6 y:4 8 12
    for(i = 1; i <= 7; i++) {
        for(j = 4; j <= 12; j += 4) {
            cursor_move(i, j);
            addch('|');
        }
    }
    for(i = 2; i <= 6; i += 2) {
        for(j = 1; j <= 15; j++) {
            cursor_move(i, j);
            addch('-');
        }
    }

// matrix initialization
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            mat[i][j] = 0;
    product_number();
    map_mat();
}

void map_mat() {
    int i, j;
    int a, b;

    a = 0;
    b = 0;
    for(i = 1; i <= 7; i += 2) {
        for(j = 3; j <= 15; j += 4) {
            if(mat[a][b] == 0)
                continue;
            mat_to_canvas(i, j, a++, b++);
        }
    }
}

void mat_to_canvas(int x, int y, int a, int b) {
    int n;
    int i, t;

    i = 0;
    n = mat[a][b];
    do {
        t = n % 10;
        cvas[x][y-i] = itoc(t); // TODO
        i++;
    } while(n /= 10);
}

char itoc(int n) {
    return '0' + n;
}

int randN(int m) {    // m% return 4
    int n;

    srand((unsigned)time(NULL));
    n = rand() % 100;
    if(n <= m) {
        return 4;
    }
    else
        return 2;
}

void product_number() {
    int n;

    judge_empty();
    srand((int)time(NULL));
    do {
        n = rand() % 15;    // n = [0,16]
    } while(*(emp+n));    // if emp(n) is empty, out
    mat[n/4][n%4-1] = randN(50);
    *(emp+n) = 1;
    empty--;
}

// judge empty pisition
void judge_empty() {
    int i, j;

    empty = 0;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            if(mat[i][j] == 0) {
                *(emp+i*4+j) = 0;
                empty++;
            }
            else
                *(emp+i*4+j) = 1;
        }
    }
    if(mode ==2) {
        for(i = 0; i < 16; i++)
            printf("%d ", *(emp+i*4+j));
        printf("\n");
    }
}

void play() {
    int i, j;
    int m;
    char ch;
    int x, y;

    while(1) {
        switch(ch=getch()) {
            case 97: 
            case 65: 
            case 27: 
/*
                judge_empty();
                // left (testing)
                for(i = 0; i < 4; i++) {
                    if(line_empty(i))
                        continue;
                    for(j = 3; j > 0; j--) {
                        if(cvas[*(a+i)][*(b+j-1)] == ' ') {
                            m = j;
                            while(m < 4) {
                                cursor_move(*(a+i), *(b+m));
                                char_move(*(a+i), *(b+m-1));
                                m++;
                            }
                        }
                        else if(cvas[*(a+i)][*(b+j-1)] == cvas[*(a+i)][*(b+j)]) {
                            cursor_move(*(a+i), *(b+j));
                            char_move(*(a+i), *(b+j-1));
                            doubleN(*(a+i), *(b+j-1));
                        }
                        else if(cvas[*(a+i)][*(b+j-1)] != cvas[*(a+i)][*(b+j)])
                            continue;
                    }
                }
                refresh();
                judge_empty();
                product_number();

                printf("left finish \n");
                break;
*/
            case 100: 
            case 68: 
            case 26: 
                // right
            case 119: 
            case 87: 
            case 24: 
                // up
            case 115: 
            case 83: 
            case 25: 
                // down
            default:
                break;
        }
    }
}

int line_empty(int r) {
    int i = 0;
    int sum = 0;

    while(i < 4) {
       sum += *(emp+4*r+i);
       i++;
    }
    return sum;
}

void doubleN(int x, int y) {
    int n;
    int i, t;

    n = (int)cvas[x][y];    // '0' is 48
    printf("%d", n);
    i = 0;
    do {
        t = n % 10;
        cvas[x][y-i] = t;
        i++;
    } while(n /= 10);
}

void game_over() {
    if(empty == 0) {
        printf("Game Over! \n");
        exit(0);
    }
}
