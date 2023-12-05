#include <pthread.h>

#include <stdio.h>

#include <stdlib.h>

typedef struct

{

    int *arr1;

    int *arr2;

    int arr1_rows;

    int arr1_cols;

    int arr2_rows;

    int arr2_cols;

} arrInfo;

arrInfo *initArrInfo(int *arr1, int *arr2, int arr1_rows, int arr1_cols, int arr2_rows, int arr2_cols)

{

    arrInfo *ainfo = (arrInfo *)malloc(sizeof(arrInfo));

    ainfo->arr1 = arr1;

    ainfo->arr2 = arr2;

    ainfo->arr1_rows = arr1_rows;

    ainfo->arr1_cols = arr1_cols;

    ainfo->arr2_rows = arr2_rows;

    ainfo->arr2_cols = arr2_cols;

    return ainfo;
}

typedef struct

{

    arrInfo *arr_info;

    int arr1Row;

    int arr2Col;

} argMulRC;

argMulRC *initArgMulRC(arrInfo *ainfo, int arr1Row, int arr2Col)

{

    argMulRC *argmulrc = (argMulRC *)malloc(sizeof(argMulRC));

    argmulrc->arr_info = ainfo;

    argmulrc->arr1Row = arr1Row;

    argmulrc->arr2Col = arr2Col;

    return argmulrc;
}

typedef struct

{

    arrInfo *arr_info;

    int arr1Row;

} argMulRA;

argMulRA *initArgMulRA(arrInfo *ainfo, int arr1Row)

{

    argMulRA *argmulra = (argMulRA *)malloc(sizeof(argMulRA));

    argmulra->arr_info = ainfo;

    argmulra->arr1Row = arr1Row;

    return argmulra;
}

// Multiply Row and Column

void *mulRC(void *argmulrc)

{

    argMulRC *arg = (argMulRC *)argmulrc;

    int *arr1 = arg->arr_info->arr1;

    int *arr2 = arg->arr_info->arr2;

    int i1 = arg->arr1Row;

    int j2 = arg->arr2Col;

    int C1 = arg->arr_info->arr1_cols;

    int C2 = arg->arr_info->arr2_cols;

    int *result = (int *)malloc(sizeof(int));

    for (int k = 0; k < C1; k++)

    {

        *result += (*(arr1 + i1 * C1 + k)) * (*(arr2 + k * C2 + j2));
    }

    return result;
}

// Multiply Row and Array

void *mulRA(void *argmulra)

{

    argMulRA *arg = (argMulRA *)argmulra;

    int *arr1 = arg->arr_info->arr1;

    int *arr2 = arg->arr_info->arr2;

    int i1 = arg->arr1Row;

    int C1 = arg->arr_info->arr1_cols;

    int C2 = arg->arr_info->arr2_cols;

    int *result = (int *)malloc(sizeof(int) * C2);

    for (int k = 0; k < C2; k++)

    {

        result[k] = 0;

        for (int j = 0; j < C1; j++)

        {

            result[k] += (*(arr1 + i1 * C1 + j)) * (*(arr2 + j * C2 + k));
        }
    }

    return result;
}

int main()

{

    int R1 = 3;

    int C1 = 2;

    int R2 = 2;

    int C2 = 4;

    int arr1[3][2] = {{1, 2},

                      {10, 4},

                      {5, 6}};

    int arr2[2][4] = {{1, 2, 3, 4},

                      {5, 6, 7, 8}};

    arrInfo *ainfo = initArrInfo((int *)arr1, (int *)arr2, R1, C1, R2, C2);

    // Multiply Each Row and Column

    int *result_rc[R1][C2];

    pthread_t threads_rc[R1][C2];

    for (int i = 0; i < R1; i++)

    {

        for (int j = 0; j < C2; j++)

        {

            pthread_create(&threads_rc[i][j], NULL, mulRC, (void *)initArgMulRC(ainfo, i, j));
        }
    }

    for (int i = 0; i < R1; i++)

    {

        for (int j = 0; j < C2; j++)

        {

            pthread_join(threads_rc[i][j], (void *)&result_rc[i][j]);
        }
    }

    for (int i = 0; i < R1; i++)

    {

        for (int j = 0; j < C2; j++)

        {

            printf("%d ", *result_rc[i][j]);
        }

        printf("\n");
    }

    printf("\n\n\n");

    // Multiply Each Row with the Array

    int *result_ra[R1];

    pthread_t threads_ra[R1];

    for (int i = 0; i < R1; i++)

    {

        pthread_create(&threads_ra[i], NULL, mulRA, (void *)initArgMulRA(ainfo, i));
    }

    for (int i = 0; i < R1; i++)

    {

        pthread_join(threads_ra[i], (void *)&result_ra[i]);
    }

    for (int i = 0; i < R1; i++)

    {

        int *row = result_ra[i];

        for (int j = 0; j < C2; j++)

        {

            printf("%d ", row[j]);
        }

        printf("\n");
    }

    exit(0);
}