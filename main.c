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

bool is_index_in_m(int rows, int cols, int index_row, int index_col) {
    if (index_row > -1 && index_row < rows
    && index_col > -1 && index_col < cols) {
        return true;
    } else {
        return false;
    }
}

size_t find_count_neighbors(matrix m, int index_row, int index_col, int
rows, int cols) {
    size_t count_neighbors = 0;
    for (int index_row_math = index_row - 1; index_row_math < index_row + 2;
         index_row_math++) {
        for (int index_col_math = index_col - 1; index_col_math < index_col + 2;
             index_col_math++) {
            if (is_index_in_m(rows, cols, index_row_math, index_col_math) &&
                m.values[index_row_math][index_col_math] &&
                (index_row != index_row_math || index_col != index_col_math)) {
                count_neighbors++;
            }
        }
    }
    return count_neighbors;
}

int cellResultByPosition(matrix m, int index_row, int index_col, size_t count_neighbors) {
    return ((m.values[index_row][index_col] == 1 &&
    (count_neighbors == 2 || count_neighbors == 3)) ||
    (m.values[index_row][index_col] == 0 && count_neighbors == 3));
}

void task_2(matrix m, matrix *newM, int rows, int cols) {
    for (int index_row = 0; index_row < rows; index_row++) {
        for (int index_col = 0; index_col < cols; index_col++) {
            size_t count_neighbors =
                    find_count_neighbors(m, index_row, index_col, rows, cols);
            newM->values[index_row][index_col] =
                    cellResultByPosition(m, index_row, index_col, count_neighbors);
        }
    }
}
//Пример из пособия
void test_task_2() {
    int rows = 4;
    int cols = 3;
    matrix original_matrix = createMatrixFromArray((int[]) {
        0, 1, 0,
        0, 0, 1,
        1, 1, 1,
        0, 0, 0},
                                         4, 3);

    matrix transformed_m = getMemMatrix(rows, cols);
    task_2(original_matrix, &transformed_m, rows, cols);

    matrix result = createMatrixFromArray((int[]) {
        0, 0, 0,
        1, 0, 1,
        0, 1, 1,
        0, 1, 0},
                                            4, 3);
    assert(areTwoMatricesEqual(&transformed_m, &result));
}

int main() {
    //test_task_1();
    test_task_2();
}

