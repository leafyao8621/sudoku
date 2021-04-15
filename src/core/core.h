#ifndef _CORE_H_
#define _CORE_H_

extern char grid[81];

void core_initialize(void);
void core_generate_grid(void);
int core_solve_grid(void);
void core_print_grid(void);
void core_modify_grid(char row, char col, char data);
int core_check(char row, char col);

#endif
