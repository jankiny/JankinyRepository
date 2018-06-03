/* Problem:
 * Win32:
 *     cursor.h: command "cls" flick in WIN.    Line ./cursor.h/: 104
 *     getch() can't work in cfree3.5, using getche() for now.   Line: 191
 *
 * Update v4.4:
 *     easy to read and mainten, modify some comment; done!
 *     modify game logic; done!
 *     add game menu;
 *     add score system;
 *     expand the canvas, Goal to 8192(2^13);
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cursor.h"

/* 每个宫格只能容纳3个字符 every cubes only have 3 character on this version */
#define GOAL 1024

char **cvas;    // Canvas
int mat[4][4];
int score = 0;

/* initialization 初始化 */
void init();    

/* maping and refresh 映射并且刷新画布 */
void cvas_refresh(); 
void map_mat();    // map matrix to canvas 
void mat_to_canvas(int x, int y, int a, int b);

/* product number in a empty position 在空位置产生数字 */
void product_number(); 
int randN(int m);    // return 4 or 2 
int judge_empty();    //judge empty 判断是否为空

/* game logic 游戏逻辑 */
void play(); 
void game_over();    // game over 

int main() 
{
    init();
    mode_change(2);
    cvas_refresh();
    play();
}

/* create a canvas; draw frame; product the begining number */
void init() 
{
    int i, j;

    cvas = canvas(7, 15);
    /* draw frame */
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

    /* matrix initialization */
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            mat[i][j] = 0;
    product_number();
}

/* refresh */
void cvas_refresh() 
{
    int i, j;

    map_mat();
    refresh();
    printf("Score: %d \n", score);
    if(mode == 1) {
        for(i = 0; i < 4; i++) { 
            for(j = 0; j < 4; j++)
                printf("%d ", mat[i][j]);
            printf(";");
        }
        printf("\n");
    }
}

/* map matrix to canvas */
void map_mat() 
{
    int i, j;
    int a, b;

    for(a = 0, i = 1; i <= 7; i += 2) {
        for(b = 0, j = 3; j <= 15; j += 4) {
            mat_to_canvas(i, j, a, b);
            b++;
        }
        a++;
    }
}

void mat_to_canvas(int x, int y, int a, int b) 
{
    int n, t;
    int i;

    i = 0;
    do {
        cursor_move(x, y-i);
        addch(' ');
        i++;
    } while(i < 3);

    if((n=mat[a][b]) == 0) {
        return;
    }
    else {
        i = 0;
        do {
            t = n % 10;
            cursor_move(x, y-i);
            addch('0'+t);
            i++;
        } while(n /= 10);
    }
}

/* m% return 4 */
int randN(int m) 
{ 
    int n;

    srand((unsigned)time(NULL));
    n = rand() % 100;
    if(n <= m) {
        return 4;
    }
    else
        return 2;
}

/* return random integer number */
void product_number() 
{
    int a, b;

    srand((int)time(NULL));
    do {
        a = rand() % 4;
        b = rand() % 4;
    } while(mat[a][b]);    // if mat[a][b] is empty, out
    mat[a][b] = randN(50);
}

/* judge empty pisition */
int judge_empty() 
{
    int i, j;
    int empty;

    empty = 0;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            if(mat[i][j] == 0) {
                empty++;
            }
            else if(mat[i][j] >= GOAL)
                game_over(1);
        }
    }
    return empty;
}

/* game logic */
void play() 
{
    int i, j, m;
    char ch;
    int empty;
    int move = 0;    //是否移动过的move_flag,用于判断是否该product_number()

    while(1) {
        switch(ch=getche()) {
            case 97: 
            case 65: 
                /* 左：行由上向下，列由右向左，进行二维地遍历 */ 
                for(i = 0; i < 4; i++) {
                    for(j = 3; j > 0; j--) {
                        /* 若本宫格非空，而下一个宫格为空，右边的宫格向左移 */
                        if(mat[i][j] != 0 && mat[i][j-1] == 0) {
                            m = j;
                            while(m < 4) {
                                /* 遇到空宫格说明左移结束(避免多余操作) */
                                if(mat[i][m] == 0)
                                    break;
                                mat[i][m-1] = mat[i][m];
                                mat[i][m] = 0;
                                m++;
                            }
                            move = 1;
                        }
                        /* 若本宫格非空，而下一个宫格与本宫格相同，会出现三种情况：
                         * 两个宫格相同； 三个宫格相同； 四个宫格相同；
                         */
                        else if(mat[i][j] != 0 && mat[i][j] == mat[i][j-1]) {
                            /* 三个宫格相同时，跳过第一个宫格，接下来两宫格相加 */
                            if(j-2 >= 0 && mat[i][j] == mat[i][j-2]) {
                                /* 四宫格相同，直接执行两宫格相加，剩余两宫格相同 */
                                if(j-3 >= 0 && mat[i][j] == mat[i][j-3]) {
                                    goto h1;
                                }
                                continue;
                            }
                            /* 两宫格相同，相加 */
                            h1:    // <--goto h1
                            mat[i][j-1] += mat[i][j];
                            mat[i][j] = 0;
                            score += mat[i][j-1];
                            /* 当两宫格相加后，应当做的是后面所有宫格向左移(例如
                             * 2 2 4 0的情况。四宫格执行一次操作后一定会出现该情况)
                            */
                            m = j+1;
                            while(m < 4) {
                                if(mat[i][m] == 0)
                                    break;
                                mat[i][m-1] = mat[i][m];
                                mat[i][m] = 0;
                                m++;
                            }
                            move = 1;
                        }
                        else {
                            continue;
                        }
                   }
                }
                /* 如果数字移动过，则产生新数字 */
                if(move) {
                    product_number();
                    move = 0;
                    cvas_refresh();
                }
                /* 如果没有移动过，则判断是否已没有位置可以移动 */
                /* BUG: 若数组全是非0，且只有左右的维度有两个相同的数，此时，
                 * 进行向左或向右的操作，可能柳暗花明又一村，但若手误或某些原因
                 * 进行了向上或向下的操作，由于move_flag为0，会判断为游戏结束
                 */
                else if((empty=judge_empty()) == 0) {
                    game_over(0);
                }
                /* 辅助debug */
                if(mode == 1)
                    printf("left finish \n");
                break;
            case 100: 
            case 68: 
                // right
                for(i = 0; i < 4; i++) {
                    for(j = 0; j < 3; j++) {
                        if(mat[i][j] != 0 && mat[i][j+1] == 0) {
                            m = j;
                            while(m >= 0) {
                                if(mat[i][m] == 0)
                                    break;
                                mat[i][m+1] = mat[i][m];
                                mat[i][m] = 0;
                                m--;
                            }
                            move = 1;
                        }
                        else if(mat[i][j] != 0 && mat[i][j] == mat[i][j+1]) {
                            if(j+2 <= 3 && mat[i][j] == mat[i][j+2]) {
                                if(j+3 <= 3 && mat[i][j] == mat[i][j+3]) {
                                    goto h2;
                                }
                                continue;
                            }
                            h2:    // goto h2
                            mat[i][j+1] += mat[i][j];
                            mat[i][j] = 0;
                            score += mat[i][j+1];
                            m = j-1;
                            while(m >= 0) {
                                if(mat[i][m] == 0)
                                    break;
                                mat[i][m+1] = mat[i][m];
                                mat[i][m] = 0;
                                m--;
                            }
                            move = 1;
                        }
                        else if(mat[i][j] == 0 || mat[i][j] != mat[i][j+1]) {
                            continue;
                        }
                   }
                }
                if(move) {
                    product_number();
                    move = 0;
                    cvas_refresh();
                }
                else if((empty=judge_empty()) == 0) {
                    game_over(0);
                }
                if(mode == 1)
                    printf("right finish \n");
                break;
            case 119: 
            case 87: 
                // up
                for(j = 0; j < 4; j++) {
                    for(i = 3; i > 0; i--){
                        if(mat[i][j] != 0 && mat[i-1][j] == 0) {
                            m = i;
                            while(m < 4) {
                                if(mat[m][j] == 0)
                                    break;
                                mat[m-1][j] = mat[m][j];
                                mat[m][j] = 0;
                                m++;
                            }
                            move = 1;
                        }
                        else if(mat[i][j] != 0 && mat[i][j] == mat[i-1][j]) {
                            if(i-2 >= 0 && mat[i][j] == mat[i-2][j]) {
                                if(i-3 >= 0 && mat[i][j] == mat[i-3][j]) {
                                    goto h3;
                                }
                                continue;
                            }
                            h3:    // goto h3
                            mat[i-1][j] += mat[i][j];
                            mat[i][j] = 0;
                            score += mat[i-1][j];
                            m = i+1;
                            while(m < 4) {
                                if(mat[m][j] == 0)
                                    break;
                                mat[m-1][j] = mat[m][j];
                                mat[m][j] = 0;
                                m++;
                            }
                            move = 1;
                        }
                        else if(mat[i][j] == 0 || mat[i][j] != mat[i-1][j]) {
                            continue;
                        }
                   }
                }
                if(move) {
                    product_number();
                    move = 0;
                    cvas_refresh();
                }
                else if((empty=judge_empty()) == 0) {
                    game_over(0);
                }
                if(mode == 1)
                    printf("up finish \n");
                break;
            case 115: 
            case 83: 
                // down
                for(j = 0; j < 4; j++) {
                    for(i = 0; i < 3; i++){
                        if(mat[i][j] != 0 && mat[i+1][j] == 0) {
                            m = i;
                            while(m >= 0) {
                                if(mat[m][j] == 0)
                                    break;
                                mat[m+1][j] = mat[m][j];
                                mat[m][j] = 0;
                                m--;
                            }
                            move = 1;
                        }
                        else if(mat[i][j] != 0 && mat[i][j] == mat[i+1][j]) {
                            if(i+2 <= 3 && mat[i][j] == mat[i+2][j]) {
                                if(i+3 <= 3 && mat[i][j] == mat[i+3][j]) {
                                    goto h4;
                                }
                                continue;
                            }
                            h4:    // goto h4
                            mat[i+1][j] += mat[i][j];
                            mat[i][j] = 0;
                            score += mat[i+1][j];
                            m = i-1;
                            while(m >= 0) {
                                if(mat[m][j] == 0)
                                    break;
                                mat[m+1][j] = mat[m][j];
                                mat[m][j] = 0;
                                m--;
                            }
                            move = 1;
                        }
                        else if(mat[i][j] == 0 || mat[i][j] != mat[i+1][j]) {
                            continue;
                        }
                   }
                }
                if(move) {
                    product_number();
                    move = 0;
                    cvas_refresh();
                }
                else if((empty=judge_empty()) == 0) {
                    game_over(0);
                }
                if(mode == 1)
                    printf("down finish \n");
                break;
            default:
                break;
        }
    }
}

void game_over(int n) 
{
    if(n == 0)
        printf("Game Over! \n");
    else if(n == 1)
        printf("win! \n");
    exit(0);
}

void game_menu() 
{
    ;
}

void score_system()
{
    ;
}
