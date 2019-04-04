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

pthread_t inputThread, displayThread,
    healthStatusHandlerThread, hungerStatusHandlerThread, hygieneStatusHandlerThread,
    bathActionHandlerThread;

int current_scene = 0;
/*
    0 -> standby scene
    1 -> battle scene
    2 -> shop scene
*/

typedef struct
{
    int max;
    int value;
    int delta;
    int delay;
} status;

typedef struct
{
    int delta;
    int amount;
    int delay;
} action;

status hunger = {200, 200, 5, 10};
status hygiene = {100, 100, 10, 30};
status health = {300, 300, -5, 10};
int enemyHealth = 0;

action food = {15, 10, 0};
action bath = {30, 0, 20};
action attack = {20, 0, 0};

int *shopStock;
key_t shopKey = 76255;
int shopID;

void *statusHandler(void *arg);
void *actionHandler(void *arg);
void *input(void *arg);
void refresh_display();
void *display(void *arg);
int getch(void);

int main()
{
    shopID = shmget(shopKey, sizeof(int), IPC_CREAT | 0666);
    shopStock = shmat(shopID, NULL, 0);
    //status handler thread
    pthread_create(&healthStatusHandlerThread, NULL, statusHandler, &health);
    pthread_create(&hungerStatusHandlerThread, NULL, statusHandler, &hunger);
    pthread_create(&hygieneStatusHandlerThread, NULL, statusHandler, &hygiene);

    //action handler thread
    pthread_create(&bathActionHandlerThread, NULL, actionHandler, &bath);

    //output thread
    pthread_create(&displayThread, NULL, display, NULL);

    //input thread
    pthread_create(&inputThread, NULL, input, NULL);

    pthread_join(healthStatusHandlerThread, NULL);
    pthread_join(hungerStatusHandlerThread, NULL);
    pthread_join(hygieneStatusHandlerThread, NULL);

    pthread_join(bathActionHandlerThread, NULL);
    pthread_join(displayThread, NULL);
    pthread_join(inputThread, NULL);
    return 0;
}

void *statusHandler(void *arg)
{
    status *temp = ((status *)arg);
    while (1)
    {
        sleep(temp->delay);
        temp->value = temp->value - temp->delta;
        if (temp->value <= 0)
        {
            printf("KALAH\n");
            exit(0);
        }
    }
}

void *actionHandler(void *arg)
{
    action *temp = ((action *)arg);
    int defaultDelta = temp->delta;
    int defaultDelay = temp->delay;

    while (1)
    {
        if (temp->delta <= 0)
        {
            temp->delay--;
            if (temp->delay == 0)
            {
                temp->delta = defaultDelta;
                temp->delay = defaultDelay;
            }
        }
        sleep(1);
    }
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

void *input(void *arg)
{
    while (1)
    {
        char input = getch();
        switch (current_scene)
        {
        case 0:
            switch (input)
            {
            case '1':
                if (food.amount > 0)
                {
                    hunger.value += food.delta;
                    if (hunger.value > hunger.max)
                        hunger.value = hunger.max;
                    food.amount--;
                    refresh_display();
                }
                break;
            case '2':
                hygiene.value += bath.delta;
                bath.delta = 0;
                if (hygiene.value > hygiene.max)
                    hygiene.value = hygiene.max;
                refresh_display();
                break;
            case '3':
                enemyHealth = 100;
                pthread_cancel(healthStatusHandlerThread);
                pthread_cancel(hygieneStatusHandlerThread);
                pthread_cancel(hungerStatusHandlerThread);
                current_scene = 1;
                refresh_display();
                break;
            case '4':
                current_scene = 2;
                refresh_display();
                break;
            case '5':
                exit(0);
                break;
            }
            break;
        case 1:
            switch (input)
            {
            case '1':
                if (enemyHealth > 0)
                {
                    health.value -= attack.delta;
                    enemyHealth -= attack.delta;
                    if (health.value <= 0)
                    {
                        printf("KALAH\n");
                        exit(0);
                    }
                    if (enemyHealth < 0)
                        enemyHealth = 0;
                    refresh_display();
                }
                break;
            case '2':
                pthread_create(&healthStatusHandlerThread, NULL, statusHandler, &health);
                pthread_create(&hungerStatusHandlerThread, NULL, statusHandler, &hunger);
                pthread_create(&hygieneStatusHandlerThread, NULL, statusHandler, &hygiene);
                current_scene = 0;
                refresh_display();
                break;
            }
            break;
        case 2:
            switch (input)
            {
            case '1':
                if (*shopStock > 0)
                {
                    (*shopStock)--;
                    food.amount++;
                    refresh_display();
                }
                break;
            case '2':
                current_scene = 0;
                refresh_display();
                break;
            }
            break;
        }
    }
}

void refresh_display()
{
    pthread_cancel(displayThread);
    pthread_create(&displayThread, NULL, display, NULL);
}

void *display(void *arg)
{
    int time = 0;
    while (1)
    {
        system("clear");
        printf("Time: %d\n", time++);
        switch (current_scene)
        {
        case 0:
            printf("Standby Mode\n");
            printf("Health: %d\n", health.value);
            printf("Hunger: %d\n", hunger.value);
            printf("Hygiene: %d\n", hygiene.value);
            printf("Food left: %d\n", food.amount);
            if (bath.delta == 0)
                printf("Bath will be ready in: %d s\n", bath.delta == 0 ? bath.delay : 0);
            else
                printf("Bath is ready\n");
            printf("Choices\n");
            printf("1. Eat\n");
            printf("2. Bath\n");
            printf("3. Battle\n");
            printf("4. Shop\n");
            printf("5. Exit\n");
            break;
        case 1:
            printf("Battle Mode\n");
            printf("Monster's Health: %d\n", health.value);
            printf("Enemy's Health: %d\n", enemyHealth);
            if (enemyHealth <= 0)
                printf("MUSUH KALAH\n");
            printf("Choices\n");
            printf("1. Attack\n");
            printf("2. Run\n");
            break;
        case 2:
            printf("Shop Mode\n");
            printf("Shop food stock: %d\n", *shopStock);
            printf("Your food stock: %d\n", food.amount);
            printf("Choices\n");
            printf("1. Buy\n");
            printf("2. Back\n");
            break;
        }
        sleep(1);
    }
}