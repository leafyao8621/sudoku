#include <stdio.h>
#include <string.h>
#include <time.h>
#include "core.h"
#include "../util/solver.h"
#include "../util/mt19937.h"

char grid[81];
struct MT19937 gen;

static void swap(char *a, char *b) {
    char c = *a;
    *a = *b;
    *b = c;
}

void core_initialize(void) {
    mt19937_initialize(&gen, time(0));
}

void core_generate_grid(void) {
    memset(grid, 0, 81);
    char buf[9];
    for (char i = 0, *ii = buf; i < 9; *(ii++) = ++i);
    for (char cnt = 0, *iter = grid; cnt < 3; ++cnt, iter += 3) {
        for (char i = 0, *ii = buf; i < 9;
             ++i, swap(ii++, buf + mt19937_gen(&gen) % 9));
        for (char i = 0, *ii = buf; i < 3; ++i, ii += 3, iter += 9) {
            memcpy(iter, ii, 3);
        }
    }
    solver_solve(grid);
    for (char i = 0; i < 37; ++i) {
        char *cur = grid + mt19937_gen(&gen) % 81;
        for (; !*cur; cur = grid + mt19937_gen(&gen) % 81);
        *cur = 0;
    }
}

int core_solve_grid(void) {
    return solver_solve(grid);
}

void core_print_grid(void) {
    for (char i = 0, *iter = grid; i < 9; ++i) {
        for (char j = 0; j < 9; ++j, ++iter) {
            putchar(*iter + '0');
            putchar(0x20);
        }
        putchar(10);
    }
}

void core_modify_grid(char row, char col, char data) {
    grid[row * 9 + col] = data;
}

int core_check(char row, char col) {
    return solver_check(grid, row, col);
}
