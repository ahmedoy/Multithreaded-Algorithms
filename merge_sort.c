#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

typedef struct
{
    int *arr;
    int start;
    int end;
} argument;

argument **splitArg(argument *arg, int split_idx)
{
    argument **argArr = (argument **)malloc(sizeof(argument *) * 2);
    argArr[0] = (argument *)malloc(sizeof(argument));
    argArr[1] = (argument *)malloc(sizeof(argument));

    argArr[0]->arr = arg->arr;
    argArr[0]->start = arg->start;
    argArr[0]->end = split_idx;

    argArr[1]->arr = arg->arr;
    argArr[1]->start = split_idx + 1;
    argArr[1]->end = arg->end;

    return argArr;
}


void *merge_sort(void *arg_)
{
    argument *arg = (argument *)arg_;
    int low = arg->start;
    int high = arg->end;
    if (low == high)
    {
        return NULL;
    }

    int mid = (high + low) / 2;

    if (high > low + 1)
    {
        argument **new_args = splitArg(arg, mid);
        argument *arg1 = new_args[0];
        argument *arg2 = new_args[1];
        pthread_t thread1, thread2;
        pthread_create(&thread1, NULL, merge_sort, (void*)arg1);
        pthread_create(&thread2, NULL, merge_sort, (void*)arg2);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
    }

    // Merging Code
    int n1 = mid - low + 1;
    int n2 = high - mid;
    int arr1Copy[n1 + 1];
    int arr2Copy[n2 + 2];
    for(int i = 0; i<n1; i++){
        arr1Copy[i] = arg->arr[low+i];
    }
    for(int i = 0; i<n2; i++){
        arr2Copy[i] = arg->arr[mid+i+1];
    }
    arr1Copy[n1] = INT_MAX;
    arr2Copy[n2] = INT_MAX;
    int k = low;
    int i = 0;
    int j = 0;

    while(k<=high){
        if(arr1Copy[i] <= arr2Copy[j]){
            arg->arr[k++] = arr1Copy[i++];
        }
        else{
            arg->arr[k++] = arr2Copy[j++];
        }
    }
    return NULL;

}


argument *scanArr()
{
    FILE *file = fopen("input.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return NULL;
    }

    int n;
    fscanf(file, "%d", &n);
    int *arr = (int *)malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++)
    {
        fscanf(file, "%d", arr + i);
    }

    argument *arg = (argument *)malloc(sizeof(argument));
    arg->arr = arr;
    arg->start = 0;
    arg->end = n - 1;
    return arg;
}

int main()
{

    argument *arg = scanArr();
    merge_sort((void*) arg);

    for (int i = 0; i <= arg->end; i++)
    {
        printf("%d ", arg->arr[i]);
    }
    printf("\n");

    exit(0);
}