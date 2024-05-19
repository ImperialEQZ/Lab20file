#include "matrix.h"
#include "matrix.c"

void task_1(matrix *m, size_t count_query, size_t *array_query[]) {
    size_t row_1, col_1, row_2, col_2;
    for (size_t request = 0; request < count_query; request++) {

        row_1 = array_query[request][0];
        col_1 = array_query[request][1];
        row_2 = array_query[request][2];
        col_2 = array_query[request][3];

        for (size_t i_row = row_1; i_row <= row_2; i_row++)
            for (size_t idx_col = col_1; idx_col <= col_2; idx_col++)
                m->values[i_row][idx_col]++;
    }
}
//Тест из пособия
void test_task_1() {
    matrix original_matrix = createMatrixFromArray((int[]) {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0},
                                       3, 3);

    size_t count_query = 2;

    size_t query_1[4] = {1, 1, 2, 2};
    size_t query_2[4] = {0, 0, 1, 1};

    size_t *query_1_2[2] = {query_1, query_2};

    task_1(&original_matrix, count_query, query_1_2);
    matrix result = createMatrixFromArray((int[]) {
        1, 1, 0,
        1, 2, 1,
        0, 1, 1},
                                            3, 3);

    assert(areTwoMatricesEqual(&original_matrix, &result));
}

int main() {
    test_task_1();
}

