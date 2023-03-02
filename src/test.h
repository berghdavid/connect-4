#ifndef TEST_H
#define TEST_H

int get_eval(Game* g, int moves[], int len);

void test_row(Game* g);

void test_col(Game* g);

void test_diag_up(Game* g);

void test_diag_down(Game* g);

int main();

#endif
