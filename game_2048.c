#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

// game in a 16 squares for 4*4, showed by two-dimensional array,
// represented 0 by space
int a[4][4] = {0} ;
// 16 squares
int empty;
int old_y, old_x;

void draw(); // draw game interface
void play(); // game logic
void init(); //initialization function
void draw_one(int y, int x); // draw single number
void vnt_value(int *new_y, int *new_x); //
int game_over();
int cnt_one(int y, int x); //

int main()
{
    init();
    play();
    endwin();

    return 0;
}

void init()
{
    int x, y;

    initscr();
    cbreak();
    noecho();
    curs_set(0);

    empty = 15;
    srand(time(0));
    x = rand() % 4;
    y = rand() % 4;
    a[y][x] = 2;
    draw();
}

void draw()
{
    int n, m, x, y;
    char c[4] = {'0', '0', '0', '0'};

    clear();
    for (n = 0; n < 9; n += 2)    // draw horizontal line
        for (m = 0; m < 21; m++) {
            move(n, m);
            addch('-');
            refresh();
        }

    for (m = 0; m < 22; m += 5)    // draw vertial line
        for (n = 1; n < 8; n++) {
            move(n, m);
            addch('|');
            refresh();
        }

    for (y = 0; y < 4; y++)    // draw number
        for (x = 0; x < 4; x++) {
            draw_one(y, x);
        }
}

void draw_one(int y, int x)
{
    int i, m, k, j;
    char c[5] = {0x00};
    i = a[y][x];
    m = 0;
    while (i > 0) {
        j = i % 10;
        c[m++] = j + '0';
        i /= 10;
    }
    m = 0;
    k = (x + 1) * 5 - 1;
    while (c[m] != 0x00) {
        move(2*y+1, k);
        addch(c[m++]);
        k--;
    }
}
