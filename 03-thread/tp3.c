#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define SIZE (int)1e8
#define RANDOM_MAX (int)1e4
int array[SIZE];

struct timeval start, end;
typedef struct
{
    int start;
    int end;
} SubArray;

// init array with random values
void initArray()
{
    int i;
    for (i = 0; i < SIZE; i++)
    {
        array[i] = rand() % RANDOM_MAX;
    }
}

// print array
void printArray()
{
    int i;
    for (i = 0; i < SIZE; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n-------------------\n");
}

// find minimum of array
int findMin()
{
    int i, min = array[0];
    for (i = 1; i < SIZE; i++)
    {
        if (array[i] < min)
            min = array[i];
    }
    printf("MIN VALUE : %d \n", min);

    return min;
}

// find maximum of array
int findMax()
{
    int i, max = array[0];
    for (i = 1; i < SIZE; i++)
    {
        if (array[i] > max)
            max = array[i];
    }
    printf("MAX VALUE : %d \n", max);

    return max;
}

// find time between start and end with a function in parameter
long findTime(int (*ptTest)())
{

    gettimeofday(&start, NULL);
    int x = ptTest();
    gettimeofday(&end, NULL);

    return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
}

// split the array in n subarray
SubArray *splitArray(int n)
{

    int sizeOfSplittedArray = SIZE / n;
    int restOfSubArray = SIZE % n;
    SubArray *subArray = malloc(sizeof(SubArray) * n);

    int i = 0;
    while (i < n)
    {
        subArray[i].start = i * sizeOfSplittedArray;
        subArray[i].end = subArray[i].start + sizeOfSplittedArray;

        if (restOfSubArray > 0 && i == n - 1)
        {
            subArray[i].end += restOfSubArray;
        }
        i++;
    }
    return subArray;
}

// find time with n threads
long findTimeThreads(int n, int (*ptTest)())
{
    int i = 0;
    pthread_t threads[n];
    SubArray *subArrayTest = splitArray(n);

    gettimeofday(&start, NULL);
    while (i < n)
    {
        pthread_create(&threads[i], NULL, ptTest, &subArrayTest[i]);
        i++;
    }

    i = 0;
    while (i < n)
    {
        pthread_join(threads[i], NULL);
        i++;
    }
    gettimeofday(&end, NULL);

    return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

}


// gcc -pthread tp3.c 
int main(int argc, char const *argv[])
{
    initArray();
    //printArray();

    int n ;
    printf("Enter number of threads (0, 2, 4 or 8) : ");
    scanf("%d", &n);

    if (n > 0)
    {
        printf("Choice : Number of threads : %d \n", n);
        printf("MIN TIME : %ld ms \n", findTimeThreads(n, findMin));
        printf("MAX TIME : %ld ms \n", findTimeThreads(n, findMax));

    }
    else
    {
        printf("Choice : Without thread\n");
        printf("MIN TIME : %ld ms \n", findTime(findMin));
        printf("MAX TIME : %ld ms \n", findTime(findMax));
    }

    return 0;
}
