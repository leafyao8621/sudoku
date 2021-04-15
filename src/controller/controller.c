#include <ncurses.h>
#include "controller.h"
#include "../core/core.h"

#define MODE_SELECTION 0
#define PLAY 1
#define PLAY_SELECTION 2

static char *cur, menu[10], mode, row, col, temp, rem;

static void render_mode_selection(void) {
    mvaddstr(0, 0, "Sudoku");
    mvaddstr(1, 0, "1 Play");
    mvaddstr(2, 0, "2 Solve");
    move(*cur + 1, 0);
}

static void render_play(void) {
    mvaddstr(0, 0, "Sudoku");
    char *iter = grid;
    for (char i = 0; i < 9; ++i) {
        for (char j = 0; j < 9; ++j, ++iter) {
            mvaddch(i + 1, j << 1, !*iter ? '|' : ' ');
            mvaddch(i + 1, (j << 1) + 1, *iter ? *iter + '0' : '_');
        }
    }
    move(row + 1, (col << 1) + 1);
}

void controller_initialize(void) {
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    core_initialize();
    cur = menu;
    *cur = 0;
    mode = MODE_SELECTION;
    render_mode_selection();
}

void controller_finalize(void) {
    endwin();
}

int controller_handle(void) {
    int in = getch();
    switch (in) {
    case 'Q':
    case 'q':
        return 1;
    case 'W':
    case 'w':
    case KEY_UP:
        switch (mode) {
        case MODE_SELECTION:
            *cur = !*cur;
            move(*cur + 1, 0);
            break;
        case PLAY:
            row = ((row - 1) % 9 + 9) % 9;
            move(row + 1, (col << 1) + 1);
            break;
        case PLAY_SELECTION:
            *cur = ((*cur + 1) % 10 + 10) % 10;
            mvaddch(row + 1, (col << 1) + 1, *cur ? *cur + '0' : '_');
            move(row + 1, (col << 1) + 1);
            break;
        }
        break;
    case 'S':
    case 's':
    case KEY_DOWN:
        switch (mode) {
        case MODE_SELECTION:
            *cur = !*cur;
            move(*cur + 1, 0);
            break;
        case PLAY:
            row = ((row + 1) % 9 + 9) % 9;
            move(row + 1, (col << 1) + 1);
            break;
        case PLAY_SELECTION:
            *cur = ((*cur - 1) % 10 + 10) % 10;
            mvaddch(row + 1, (col << 1) + 1, *cur ? *cur + '0' : '_');
            move(row + 1, (col << 1) + 1);
            break;
        }
        break;
    case 'A':
    case 'a':
    case KEY_LEFT:
        switch (mode) {
        case PLAY:
            col = ((col - 1) % 9 + 9) % 9;
            move(row + 1, (col << 1) + 1);
            break;
        }
        break;
    case 'D':
    case 'd':
    case KEY_RIGHT:
        switch (mode) {
        case PLAY:
            col = ((col + 1) % 9 + 9) % 9;
            move(row + 1, (col << 1) + 1);
            break;
        }
        break;
    case 'Z':
    case 'z':
    case 'J':
    case 'j':
        switch (mode) {
        case MODE_SELECTION:
            mode = PLAY;
            row = 0;
            col = 0;
            rem = 37;
            clear();
            core_generate_grid();
            render_play();
            break;
        case PLAY:
            if ((mvinch(row + 1, (col << 1)) & A_CHARTEXT) != '|') {
                move(row + 1, (col << 1) + 1);
                break;
            }
            temp = grid[row * 9 + col];
            *(++cur) = 1;
            mode = PLAY_SELECTION;
            mvaddch(row + 1, (col << 1) + 1, '1');
            move(row + 1, (col << 1) + 1);
            break;
        case PLAY_SELECTION:
            core_modify_grid(row, col, *cur);
            if (core_check(row, col)) {
                core_modify_grid(row, col, temp);
                mvaddstr(10, 0, "Invalid!");
                getch();
                mvaddstr(10, 0, "        ");
                move(row + 1, (col << 1) + 1);
                break;
            }
            mode = PLAY;
            --cur;
            if (!(--rem)) {
                mvaddstr(10, 0, "Clear!");
                getch();
                row = 0;
                col = 0;
                rem = 37;
                clear();
                core_generate_grid();
                render_play();
            }
            break;
        }
        break;
    case 'X':
    case 'x':
    case 'K':
    case 'k':
        switch (mode) {
        case MODE_SELECTION:
            break;
        case PLAY:
            mode = MODE_SELECTION;
            clear();
            render_mode_selection();
            break;
        case PLAY_SELECTION:
            mode = PLAY;
            --cur;
            mvaddch(row + 1, (col << 1) + 1, temp ? temp + '0' : '_');
            break;
        }
        break;
    }
    return 0;
}
