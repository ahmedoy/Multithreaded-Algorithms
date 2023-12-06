#include <pthread.h>



#include <stdio.h>



#include <stdlib.h>



#include <sys/time.h>



#include <string.h>



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



arrInfo *scanArrays()

{

    char filename[100];

    printf("Enter File name: ");

    fgets(filename, sizeof(filename), stdin);    

    

    if (filename[strlen(filename) - 1] == '\n') { //Replace newline with string terminatior

        filename[strlen(filename) - 1] = '\0';

    }

    

    

    FILE *file = fopen(filename, "r");

    if (file == NULL)

    {

        printf("Error opening file.\n");

        return NULL;

    }



    int R1, C1;

    fscanf(file, "%d %d", &R1, &C1);

    int *arr1 = (int *)malloc(sizeof(int) * R1 * C1);

    for (int i = 0; i < R1; i++)

    {

        for (int j = 0; j < C1; j++)

        {

            fscanf(file, "%d", arr1 + i * C1 + j);

        }

    }



    int R2, C2;

    fscanf(file, "%d %d", &R2, &C2);

    int *arr2 = (int *)malloc(sizeof(int) * R2 * C2);



    for (int i = 0; i < R2; i++)

    {

        for (int j = 0; j < C2; j++)

        {

            fscanf(file, "%d", arr2 + i * C2 + j);

        }

    }



    return initArrInfo(arr1, arr2, R1, C1, R2, C2);

}



int main()



{

    struct timeval start, end;

    double elapsed_time;

    arrInfo *ainfo = scanArrays();

    int R1 = ainfo->arr1_rows;

    int C1 = ainfo->arr1_cols;

    int R2 = ainfo->arr2_rows;

    int C2 = ainfo->arr2_cols;



    // Multiply Each Row and Column

    gettimeofday(&start, NULL);

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

    gettimeofday(&end, NULL);



    for (int i = 0; i < R1; i++)



    {



        for (int j = 0; j < C2; j++)



        {



            printf("%d ", *result_rc[i][j]);

        }



        printf("\n");

    }

    elapsed_time = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000000.0);

    printf("END1 %f\n", elapsed_time);



    // Multiply Each Row with the Array

    gettimeofday(&start, NULL);

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

    gettimeofday(&end, NULL);



    for (int i = 0; i < R1; i++)



    {



        int *row = result_ra[i];



        for (int j = 0; j < C2; j++)



        {



            printf("%d ", row[j]);

        }



        printf("\n");

    }

    elapsed_time = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000000.0);

    printf("END2 %f\n", elapsed_time);



    exit(0);

}