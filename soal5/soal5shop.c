#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <assert.h>
#include <sys/ipc.h>
#include <sys/shm.h>

pthread_t inputThread, displayThread;

int *shopStock;
key_t shopKey = 76255;
int shopID;

void *input(void *arg);
void *display(void *arg);
void refresh_display();
int getch(void);

int main()
{
    shopID = shmget(shopKey, sizeof(int), IPC_CREAT | 0666);
    shopStock = shmat(shopID, NULL, 0);

    pthread_create(&inputThread, NULL, input, NULL);
    pthread_create(&displayThread, NULL, display, NULL);

    pthread_join(displayThread, NULL);
    pthread_join(inputThread, NULL);
}

void *input(void *arg)
{
    while (1)
    {
        char input = getch();
        switch (input)
        {
        case '1':
            (*shopStock)++;
            refresh_display();
            break;
        case '2':
            exit(0);
            break;
        }
    }
}

void *display(void *arg)
{
    int time = 0;
    while (1)
    {
        system("clear");
        printf("Time: %d\n", time++);
        printf("Shop\n");
        printf("Food stock: %d\n", *shopStock);
        printf("Choices\n");
        printf("1. Restock\n");
        printf("2. Exit\n");
        sleep(1);
    }
}

void refresh_display()
{
    pthread_cancel(displayThread);
    pthread_create(&displayThread, NULL, display, NULL);
}

int getch(void)
{
    int c = 0;

    struct termios org_opts, new_opts;
    int res = 0;
    //-----  store old settings -----------
    res = tcgetattr(STDIN_FILENO, &org_opts);
    assert(res == 0);
    //---- set new terminal parms --------
    memcpy(&new_opts, &org_opts, sizeof(new_opts));
    new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    c = getchar();
    //------  restore old settings ---------
    res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
    assert(res == 0);
    return (c);
}