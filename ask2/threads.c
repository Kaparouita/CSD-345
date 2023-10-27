#include <stdio.h>
#include "threads.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

sem_t sem;

void *thread_function(void *arg)
{
   // char *student_AM = (char *)arg; //TODO: check if needed

    student *s = (student *)malloc(sizeof(student));
    s->AM =  (char *)arg;
    s->state = STUDYING;
    s->study_time = random_number(1, 5); //TODO: replace with 3-15

    // printf("Thread %d: AM: %s, State: %d, Study time: %d sec\n", s->thread_id, s->AM, s->state, s->study_time);

    // Study
    printf("%s : start studying \n", s->AM);
    sleep(s->study_time);
    // Leave
    printf("%s : stop studying \n", s->AM);

    
    // Update state
    s->state = FINISHED;
    sem_wait(&sem);
    pthread_exit(NULL);
}

int workflow_manager(int total_threads){
    int i ;
    int max_students = 8;
    int sem_counter = 0;
    pthread_t threads[total_threads]; // Array to store thread IDs TODO: add threads from cmd
    char** students = init_students(total_threads);

    for (i = 0; i < total_threads; i++)
    {
       // sem_wait(&sem);
        max_students--;
        sem_post(&sem);

        if (pthread_create(&threads[i], NULL, thread_function,students[i]) != 0)
        {
            perror("Thread creation failed");
            return 1;
        }
        if (sem_getvalue(&sem, &sem_counter) == 0 && sem_counter == 8)
        {
            // Wait for a threads to finish
            while(sem_getvalue(&sem, &sem_counter) == 0 && sem_counter != 0)
            {
                printf("Waiting for threads to finish %d\n",sem_counter);
                sleep(1);
            }
            printf("-----------------------------------------\n");
            max_students = 8; // reset max_students counter
        }
    }

    // Wait for all threads to finish
    for (i = 0; i < total_threads; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Thread join failed");
            return 1;
        }
    }
    return 0;
}


int main()
{
    sem_init(&sem, 0, 0);
    workflow_manager(20);
    return 0;
}



char** init_students(int max_students){
    char** student_data = (char**)malloc(max_students * sizeof(char*));
    //int student_count = 0;

    FILE *file = fopen("student_data.csv", "r");
    if (file == NULL) {
        perror("Error opening file");
        return student_data;
    }

    // while (student_count < max_students) {
    //     student_data[student_count] = (char *)malloc(10 * sizeof(char));
    //     if (fscanf(file, " %9[^,],", student_data[student_count]) == 1) {
    //         student_count++;
    //     } else 
    //         break; // EOF or error  
    // }
    for (int i = 0; i < max_students; i++)
    {
        student_data[i] = (char *)malloc(10 * sizeof(char)); 
        sprintf(student_data[i] , "%d",i);
    }

    fclose(file);
    return student_data;
}

int random_number(int min, int max)
{
    return (rand() % max) + min;
}