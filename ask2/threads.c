#include <stdio.h>
#include "threads.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

sem_t sem;
int sem_counter = 0;

void *thread_function(void *arg)
{
    int thread_num = (int)pthread_self(); // Get the thread's ID

    // You can add any thread-specific code here.
    student *s = (student *)malloc(sizeof(student));
    s->AM = "csd4300";
    s->state = STUDYING;
    s->study_time = random_number(1, 10);
    s->thread_id = thread_num;

    printf("Thread %d: AM: %s, State: %d, Study time: %d sec\n", s->thread_id, s->AM, s->state, s->study_time);

    printf("%s : start studying \n", s->AM);
    sleep(s->study_time);
    pthread_exit(NULL);
    sem_wait(&sem);
    sem_counter--;
    sem_destroy(&sem);
    printf("%s : stop studying \n", s->AM);
}

int main()
{
    int i;
    sem_init(&sem, 0, 1);

    pthread_t threads[10]; // Array to store thread IDs TODO: add threads from cmd

    for (i = 0; i < 10; i++)
    {
        sem_wait(&sem);
        sem_counter++;
        sem_post(&sem);

        if (pthread_create(&threads[i], NULL, thread_function,NULL) != 0)
        {
            perror("Thread creation failed");
            return 1;
        }
    }

    // Wait for all threads to finish
    for (i = 0; i < 10; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Thread join failed");
            return 1;
        }
    }

    return 0;
}

int check_sem()
{
    if (sem_counter < 9)
    {
        return 1;
    }
    return 0;
}

int random_number(int min, int max)
{
    return (rand() % max) + min;
}