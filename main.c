#include "matrix.h"
#include "matrix.c"
#include <string.h>
#include "vector.h"
#include <conio.h>

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

int sorting_numbers_comp(const void *first_number, const void *second_number) {
    return (*(int *) first_number - *(int *) second_number);
}
void fillingNumFrameSorted(int *arr, matrix m, int index_row, int index_col) {
    int index_arr = 0;
    for (size_t m_index_row = index_row - 1; m_index_row < index_row + 2; m_index_row++) {
        for (size_t m_index_col = index_col - 1; m_index_col < index_col + 2; m_index_col++) {
            if (m_index_row != index_col || m_index_col != index_col)
                arr[index_arr++] = m.values[m_index_row][m_index_col];
        }
    }

    qsort(arr, 8, sizeof(int), sorting_numbers_comp);
}

void task_3(matrix *m, int filter_size) {
    int frame[8];
    for (int index_row = 1; index_row < filter_size - 1; index_row++) {
        for (int index_col = 1; index_col < filter_size - 1; index_col++) {
            fillingNumFrameSorted(frame, *m, index_row, index_col);
            int median_m = (frame[3] + frame[4]) / 2;
            m->values[index_row][index_col] = median_m;
        }
    }
}
//Пример из пособия
void test_task_3() {
    int filter = 3;
    matrix original_matrix = createMatrixFromArray((int[]) {
        10, 20, 30,
        25, 35, 45,
        15, 25, 35},
                                       3, 3);

    task_3(&original_matrix, filter);

    matrix transformed_matrix = createMatrixFromArray((int[]) {
        10, 20, 30,
        25, 25, 45,
        15, 25, 35},
                                            3, 3);
    assert(areTwoMatricesEqual(&original_matrix, &transformed_matrix));
}

typedef struct domain {
    size_t visits_site;
    char name_site[1024];
} domain;

size_t find_domain_in_results(const domain *results, size_t size, char *s) {
    for (size_t index = 0; index < size; index++) {
        if (strcmp(results[index].name_site, s) == 0) {
            return index;
        }
    }
    return size;
}

bool find_number_in_arr(const size_t array[], size_t length, size_t num) {
    for (size_t index = 0; index < length; index++) {
        if (num == array[index])
            return true;
    }
    return false;
}
void handlerDotPrtNotNull(domain *array, size_t index, char *dotPtr,
                          domain results[], size_t *sizeResult) {
    strcpy(array[index].name_site, dotPtr + 1);
    size_t position = find_domain_in_results(results, *sizeResult, array[index].name_site);
    if (position == *sizeResult) {
        results[*sizeResult] = array[index];
        *sizeResult += 1;
    } else {
        results[position].visits_site += array[index].visits_site;
    }
}

void output_arrays_domainss(domain *results, size_t size) {
    for (size_t index = 0; index < size; index++) {
        printf("%zd %s\n", results[index].visits_site, results[index].name_site);
    }
}

void task_4(domain arr[], size_t size) {
    size_t close_index_sise[size];
    size_t count_close = 0;
    domain results[1024];
    size_t size_res = 0;
    for (size_t ind = 0; ind < size; ind++)
        results[size_res++] = arr[ind];
    while (count_close != size) {
        for (size_t index = 0; index < size; index++) {
            if (!find_number_in_arr(close_index_sise, count_close, index)) {
                char *dot_ptr;//точка
                dot_ptr = strchr(arr[index].name_site, '.');
                if (dot_ptr != NULL)
                    handlerDotPrtNotNull(arr, index, dot_ptr, results, &size_res);
                else
                    close_index_sise[count_close++] = index;
            }
        }
    }
    output_arrays_domainss(results, size_res);
}
//Пример из пособия №1
void test_task_4_1() {
    size_t size = 1;
    domain arr[1] = {{9001, "discuss.codeforces.com"}};

    task_4(arr, size);
}
//Пример из пособия №2
void test_task_4_2() {
    size_t size = 4;
    domain arr[4] = {{900, "google.mail.com"},
                       {50, "yahoo.com"},
                       {1, "intel.mail.com"},
                       {5, "wiki.org"}};
    task_4(arr, size);
}

void test_task_4_all() {
    test_task_4_1();
    test_task_4_2();
}

void find_calculated_matrix(matrix m, matrix *calculated_matrix, int rows, int cols) {
    for (size_t index_row = 0; index_row < rows; index_row++) {
        for (size_t index_col = 0; index_col < cols; index_col++) {
            if (m.values[index_row][index_col] == 1) {
                if (index_row != 0) {
                    calculated_matrix->values[index_row][index_col] =
                            calculated_matrix->values[index_row - 1][index_col] + 1;
                } else {
                    calculated_matrix->values[index_row][index_col] = 1;
                }
            } else {
                calculated_matrix->values[index_row][index_col] = 0;
            }
        }
    }
}

void task_5(matrix m, int rows, int cols, size_t *result) {
    matrix calculated_matrix = getMemMatrix(rows, cols);
    find_calculated_matrix(m, &calculated_matrix, rows, cols);
    size_t calculated_res = 0;
    for (size_t index_col = 0; index_col < cols; index_col++) {
        for (size_t index_row = 0; index_row < rows; index_row++) {
            for (size_t index_k = index_col + 1; index_k < cols + 1; index_k++) {
                int min = calculated_matrix.values[index_row][index_col];
                for (size_t index_Col_k = index_col; index_Col_k < index_k;
                     index_Col_k++) {
                    if (calculated_matrix.values[index_row][index_Col_k] < min)
                    min = calculated_matrix.values[index_row][index_Col_k];
                }
                calculated_res += min;
            }
        }
    }
    *result = calculated_res;
}
//Пример из пособия
void test_task_5_1() {
    int rows_1 = 3;
    int cols_1 = 3;
    matrix m_1 = createMatrixFromArray((int[]) {
        1, 0, 1,
        1, 1, 0,
        1, 1, 0},
                                      3, 3);
    size_t answer_1 = 0;

    task_5(m_1, rows_1, cols_1, &answer_1);

    assert(answer_1 == 13);

}
//Мой пример
void test_task_5_2() {
    int rows_2 = 3;
    int cols_2 = 3;
    matrix m_2 = createMatrixFromArray((int[]) {
        1, 1, 0,
        1, 1, 1,
        0, 0, 0},
                                      3, 3);
    size_t answer_2 = 0;

    task_5(m_2, rows_2, cols_2, &answer_2);

    assert(answer_2 == 12);
}
//Пример без единиц
void test_task_5_3() {
    int rows_3 = 2;
    int cols_3 = 3;
    matrix m_3 = createMatrixFromArray((int[]) {
        0, 0, 0,
        0, 0, 0},
                                      2, 3);
    size_t answer_3 = 0;

    task_5(m_3, rows_3, cols_3, &answer_3);

    assert(answer_3 == 0);
}

void test_task_5_all() {
    test_task_5_1();
    test_task_5_2();
    test_task_5_3();
}

void task_6(const char *str, size_t length, char *result, size_t *res_length) {
    char buff[10];
    size_t buff_length = 0;
    size_t calculated_res_length = 0;
    char num = '1';
    for (size_t index = 0; index < length; index++) {
        buff[buff_length++] = num++;
        if (str[index] == 'I')
            while (buff_length > 0)
                result[calculated_res_length++] = buff[--buff_length];
    }
    buff[buff_length++] = num;
    while (buff_length > 0)
        result[calculated_res_length++] = buff[--buff_length];
    result[calculated_res_length] = '\0';
    *res_length = calculated_res_length;
}
//Пример из пособия №1
void test_task_6_1() {
    char s[10] = "IIIDIDDD";
    size_t length = 8;
    char res[10];

    size_t res_length;

    task_6(s, length, res, &res_length);
    char answer[10] = "123549876";

    assert(strcmp(res, answer) == 0);
}
//Пример из пособия №2
void test_task_6_2() {
    char s[5] = "DDD";
    size_t length = 3;
    char res[5];

    size_t res_length;

    task_6(s, length, res, &res_length);
    char answer[10] = "4321";

    assert(strcmp(res, answer) == 0);
}

void test_task_6_all() {
    test_task_6_1();
    test_task_6_2();
}

typedef struct tree_node {
    int value;
    struct tree_node* left;
    struct tree_node* right;
} tree_node;

tree_node* helper_task_7(int *numbers, int left, int right) {
    if (left > right) {
        return NULL;
    }
    int max_index = left;
    for (int i = left; i <= right; i++) {
        if (numbers[i] > numbers[max_index]) max_index = i;
    }

    tree_node* root = (tree_node*)malloc(sizeof(tree_node));
    root->value = numbers[max_index];
    root->left = helper_task_7(numbers, left, max_index - 1);
    root->right = helper_task_7(numbers, max_index + 1, right);
    return root;
}

tree_node* task_7(int* numbers, int numbers_size) {
    return helper_task_7(numbers, 0, numbers_size - 1);
}

void print_tree_task_7(tree_node* root) {
    if (root != NULL) {
        printf("%d ", root->value);
        print_tree_task_7(root->left);
        print_tree_task_7(root->right);
    } else {
        printf("null ");
    }
}
//Пример из пособия №1
void test_task_7_1() {
    int arr[3] = {3, 2, 1};
    int len = 3;
    tree_node* res = task_7(arr, len);
    print_tree_task_7(res);
}
//Пример из пособия №2
void test_task_7_2() {
    int arr[6] = {3, 2, 1, 6, 0, 5};
    int len = 6;
    tree_node* res = task_7(arr, len);
    print_tree_task_7(res);
}

void test_task_7_all(){
    test_task_7_1();
    test_task_7_2();
}

void task_8(const char *s, size_t length,
            const size_t *indexes, char *new_str) {
    for (size_t index = 0; index < length; index++) {
        new_str[index] = s[indexes[index]];
    }
    new_str[length] = '\0';
}
//Пример из пособия №1
void test_task_8_1() {
    char str[4] = "abc";//4 т.к \0 в конце
    size_t length_str = 3;
    size_t indices[3] = {0, 1, 2};
    char resulting_str[4];//4 т.к \0 в конце

    task_8(str, length_str, indices, resulting_str);

    char answer[4] = "abc";//4 т.к \0 в конце
    assert(strcmp(resulting_str, answer) == 0);
}
//Пример из пособия №2
void test_task_8_2() {
    char str[5] = "abap";//5 т.к \0 в конце
    size_t length_str = 4;
    size_t indices[4] = {0, 3, 2, 1};
    char resulting_str[5];//5 т.к \0 в конце

    task_8(str, length_str, indices, resulting_str);

    char answer[5] = "apab";//5 т.к \0 в конце
    assert(strcmp(resulting_str, answer) == 0);
}
//Мой пример, проверка с пустой строкой
void test_task_8_3_void() {
    char str[1] = "\0";
    size_t length_str = 0;
    size_t indices[0] = {};
    char resulting_str[1];

    task_8(str, length_str, indices, resulting_str);

    char answer[1] = "";
    assert(strcmp(resulting_str, answer) == 0);
}

void test_task_8_all() {
    test_task_8_1();
    test_task_8_2();
    test_task_8_3_void();
}

FILE *open_file(char *file_name, char *action_on_file) {
    FILE *file = fopen(file_name, action_on_file);
    if (file == NULL) {
        printf("File opening error\n");
        exit(1);
    }
    return file;
}

void writing_to_file(int *nums_arr, size_t length, char *file_name) {
    FILE *file = open_file(file_name, "w");
    for (size_t index = 0; index < length; index++) {
        fprintf(file, "%d ", nums_arr[index]);
    }
    fclose(file);
}
void filter_reading_and_writing_file(vector *vec, char *r_file_name, int N, char *w_file_name) {
    FILE *r_file = open_file(r_file_name, "r");
    FILE *w_file = open_file(w_file_name, "w");
    int number;
    while (fscanf(r_file, "%d", &number) == 1) {
        if (number < N) {
            pushBackVector(vec, number);
            fprintf(w_file, "%d ", number);
        }
    }
    fclose(r_file);
    fclose(w_file);
}

void task_9(int *nums_arr, size_t length_arr, int controlNum_N, char
*first_file_name, char *second_file_name, vector *vec) {

    writing_to_file(nums_arr, length_arr, first_file_name);

    filter_reading_and_writing_file(vec, first_file_name,
                                    controlNum_N,second_file_name);
    shrinkToFit(vec);
}

void test_task_9() {
    int supplied_arr[9] = {3, 6, 17, 20, 8, 11, 32, 9, 10} ;
    size_t length_arr = 9;
    int N = 10;//10 не меньше 10
    char *first_file = "20_task_9_first_file";
    char *second_file = "20_task_9_second_file";
    vector v = createVector(10);
    task_9(supplied_arr, length_arr, N,
           first_file, second_file, &v);

    int answer_arr[4] = {3, 6, 8, 9};
    size_t answer_length = 4;
    assert(answer_length == v.size);
    //Проверю не только значения, но и позиции
    for (size_t index = 0; index < answer_length; index++) {
        assert(v.data[index] == answer_arr[index]);
    }
}

void opening_file_with_text(char *file_name, char *text) {
    FILE *file = open_file(file_name, "w");
    fprintf(file, "%s", text);
    fclose(file);
}

void task_10(char *file_name, size_t count_output_lines, char *text) {
    opening_file_with_text(file_name, text);
    FILE *file = open_file(file_name, "r");
    char line[128];
    size_t counter = 0;
    while (fgets(line, 128, file) != NULL) {
        printf("%s", line);
        counter++;
        if (counter == count_output_lines) {
            printf("Press Ctrl + C for showing\n");
            while (getch() != 3);//Чтение символа с консоли
        }
    }
}
void test_task_10_1() {
    char *file_name = "20_task_10";
    size_t count_output_lines = 3;
    char text[100] = "BIM\nBAM\nBUM\n???";
    task_10(file_name, count_output_lines, text);
}

void test_task_10_2() {
    char *file_name = "20_task_10_test_2";
    size_t count_output_lines = 4;
    char text[100] = "1234\n5678\n9100\n1477";
    task_10(file_name, count_output_lines, text);
}

void test_task_10_3() {
    char *file_name = "20_task_10_test_3_void";
    size_t count_output_lines = 1;
    char text[100] = "\0";
    task_10(file_name, count_output_lines, text);
}

void test_task_10_all() {
    test_task_10_1();
    test_task_10_2();
    test_task_10_3();
}

void task_11(char* *dict, char* *requests, int *num_in_req, int size_N, int ammount_requests_Q) {
//char* *dict - массив указателей на строки, где каждый указатель указывает на начало строки
//char* *requests - аналогично с char* *dict
    int count_str = 0;//Подсчет количества строк, в которых найдена подстрока из запроса
    int count_substring = 0;//Количество подстрок из запроса, найденных в строке
    for (int i = 0; i < ammount_requests_Q; i++) {
        for (int j = 0; j < size_N; j++) {
            if (strstr(dict[j], requests[i]) != NULL) {
                count_str++;
                count_substring++;
            } else
                count_str++;
            if (count_substring == num_in_req[i]) {
                printf("%d\n", count_str);
                break;
            }
        }
        if (num_in_req[i] > count_substring) {
            printf("-1\n");
            count_str = 0;
            count_substring = 0;
        } else {
            count_str = 0;
            count_substring = 0;
        }
    }
}
//Пример из пособия
void test_task_11() {
    int N = 10;
    int Q = 3;
    char *dict[100] = {"aa", "aaa", "aab", "ab", "abc", "ac",
                       "ba", "daa", "dab", "dadba"};
    char *requests[100] = {"a", "da", "da"};
    int answer_in_requests[3] = {4, 2, 4};
    task_11(dict, requests, answer_in_requests, N, Q);
}
void test_lab_20(){
    test_task_1();
    test_task_2();
    test_task_3();
    test_task_4_all();
    test_task_5_all();
    test_task_6_all();
    test_task_7_all();
    test_task_8_all();
    test_task_8_all();
    test_task_9();
    test_task_10_all();
    test_task_11();
}

int main() {
    test_lab_20();
    return 0;
}

