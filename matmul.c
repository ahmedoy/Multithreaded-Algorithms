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

int main()

{

    int R1 = 3;

    int C1 = 2;

    int R2 = 2;

    int C2 = 4;

    pthread_t threads[R1][C2];

    int arr1[3][2] = {{1, 2},

                      {3, 4},

                      {5, 6}};

    int arr2[2][4] = {{1, 2, 3, 4},

                      {5, 6, 7, 8}};

    int *result[R1][C2];

    arrInfo *ainfo = initArrInfo((int *)arr1, (int *)arr2, R1, C1, R2, C2);

    for (int i = 0; i < R1; i++)

    {

        for (int j = 0; j < C2; j++)

        {

            pthread_create(&threads[i][j], NULL, mulRC, (void *)initArgMulRC(ainfo, i, j));
        }
    }

    for (int i = 0; i < R1; i++)

    {

        for (int j = 0; j < C2; j++)

        {

            pthread_join(threads[i][j], (void *)&result[i][j]);
        }
    }

    for (int i = 0; i < R1; i++)

    {

        for (int j = 0; j < C2; j++)

        {

            printf("%d ", *result[i][j]);
        }

        printf("\n");
    }

    exit(0);
}