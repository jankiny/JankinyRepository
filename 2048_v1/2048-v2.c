#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cursor.h"

char **cvas;    // Canvas
int mat[4][4];
//int a[4], b[4];    // save cooedinate
int empty = 16;    // use to judge gameover

void init();    // initialization
void cvas_refresh();    // maping, and refresh
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
    cvas_refresh();
    play();
}

// create a canvas, and draw line, product begining number
void init() {
    int i, j;

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
}

void cvas_refresh() {
    int i, j;

    map_mat();
    refresh();
    if(mode == 2) {
        for(i = 0; i < 4; i++) { 
            for(j = 0; j < 4; j++)
                printf("%d ", mat[i][j]);
            printf(";");
        }
        printf("\n");    // 'clear' will remain current line
    }

}

void map_mat() {
    int i, j;
    int a, b;

    for(a = 0, i = 1; i <= 7; i += 2) {
        for(b = 0, j = 3; j <= 15; j += 4) {
            if(mat[a][b] == 0) {
                b++;
                continue;
            }
            mat_to_canvas(i, j, a, b);
            b++;
        }
        a++;
    }
}

void mat_to_canvas(int x, int y, int a, int b) {
    int n, t;
    int i;

    i = 0;
    n = mat[a][b];
    do {
        t = n % 10;
        cursor_move(x, y-i);
        addch(itoc(t));
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
    int a, b;

    srand((int)time(NULL));
    do {
        a = rand() % 4;    // n = [0,16]
        b = rand() % 4;
    } while(mat[a][b]);    // if mat[a][b] == 0, out
    mat[a][b] = randN(50);
    empty--;
}

/*
// judge empty pisition
void judge_empty() {
    int i, j;

    empty = 0;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            if(mat[i][j] == 0) {
                empty++;
            }
        }
    }
}
*/

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
                // left (testing)
                for(i = 0; i < 4; i++) {
                    for(j = 3; j >= 0; j--) {
                        if(j != 0 && mat[i][j] != mat[i][j-1]) {
                            continue;
                        }
                        else if(j != 0 && mat[i][j] == mat[i][j-1]) {
                            mat[i][j-1] += mat[i][j];
                            mat[i][j] = 0;
                        }
                        else if(mat[i][j] = 0) {
                            m = j+1;
                            while(m < 4) {
                                mat[i][m-1] = mat[i][m];
                                mat[i][m] = 0;
                                m++;
                            }
                        }
                    }
                }
                product_number();
                cvas_refresh();

                printf("left finish \n");
                break;

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

void game_over() {
    if(empty == 0) {
        printf("Game Over! \n");
        exit(0);
    }
}
