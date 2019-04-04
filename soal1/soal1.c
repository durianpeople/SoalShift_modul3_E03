#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t threads[500];
int list[500];

int ambil = -1;
int cetak = 0;

int faktorial(int n)
{
    int result = 1;
    for (int i = 2; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

void *thread(void *arg)
{
    int self_ambil = ++ambil;
    int hasil = faktorial(list[ambil]);
    // printf("Aku thread ke-%d\n", ambil);
    sleep(self_ambil);
    printf("%d\n", hasil);
    // printf("ambil: %d\n", ambil);
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void sort(int arr[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)

        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}

int main(int argc, char const *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        list[i - 1] = atoi(argv[i]);
    }
    sort(list, argc - 1);
    for (int i = 1; i < argc; i++)
    {
        int err = pthread_create(&(threads[i - 1]), NULL, &thread, NULL);
    }
    for (int i = 1; i < argc; i++)
    {
        pthread_join(threads[i - 1], NULL);
    }
}