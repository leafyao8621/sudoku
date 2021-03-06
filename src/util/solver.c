#include <string.h>
#include "solver.h"

static int check(char *grid, char *cur) {
    char val = *cur;
    char idx = cur - grid;
    char row = idx / 9;
    char col = idx % 9;
    char *square = grid + row / 3 * 27 + col / 3 * 3;
    for (char i = 0, *ii = grid + row * 9; i < 9; ++i, ++ii) {
        if ((ii != cur) && (*ii == val)) {
            return 1;
        }
    }
    for (char i = 0, *ii = grid + col; i < 9; ++i, ii += 9) {
        if ((ii != cur) && (*ii == val)) {
            return 1;
        }
    }
    for (char i = 0, *ii = square; i < 3; ++i, ii += 6) {
        for (char j = 0; j < 3; ++j, ++ii) {
            if ((ii != cur) && (*ii == val)) {
                return 1;
            }
        }
    }
    return 0;
}

int solver_check(char *grid, char row, char col) {
    return grid[row * 9 + col] && check(grid, grid + row * 9 + col);
}

int solver_solve(char *grid) {
    char workspace[81], *empty[81], **cur;
    memcpy(workspace, grid, 81);
    cur = empty;
    for (char i = 0, *ii = workspace; i < 81; ++i, ++ii) {
        if (!*ii) {
            *(cur++) = ii;
        }
    }
    *cur = 0;
    for (cur = empty; *cur;) {
        for (++(**cur); **cur < 10 && check(workspace, *cur); ++(**cur));
        if (**cur >= 10) {
            if (cur == empty) {
                return 1;
            }
            **(cur--) = 0;
        } else {
            ++cur;
        }
    }
    memcpy(grid, workspace, 81);
    return 0;
}
