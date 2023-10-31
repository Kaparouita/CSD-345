#include <stdio.h>
#include "threads.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

room *reading_room ;

void *thread_function(void *arg)
{
   // char *student_AM = (char *)arg; //TODO: check if needed
    int sem_value;

    student *s = (student *)malloc(sizeof(student));
    s->AM =  (char *)arg;
    s->state = WAITING;
    s->study_time = random_number(2, 3); //TODO: replace with 3-15

    // Enter
    reading_room->students[reading_room->curr_students] = (student *)malloc(sizeof(student));
    reading_room->students[reading_room->curr_students] = s;
    reading_room->curr_students++;
    print_room(reading_room);

    sem_getvalue(&reading_room->sem, &sem_value);

    if(sem_value == 0){
        reading_room->is_full = 1;
    }
    while(sem_value == 0 || reading_room->is_full == 1){
        sleep(1);
        sem_getvalue(&reading_room->sem, &sem_value);
        if(sem_value == 0){
            reading_room->is_full = 1;
        }
    }
    sem_wait(&reading_room->sem);


    // Study
    printf("%s : start studying \n", s->AM);
    sleep(s->study_time);
    
    // Leave
    printf("%s : stop studying \n", s->AM);

    
    // Update state
    s->state = FINISHED;
    sem_post(&reading_room->sem);
    sem_getvalue(&reading_room->sem, &sem_value);
    print_room(reading_room);

    if(sem_value == 8){
        reading_room->is_full = 0;
    }
    reading_room->curr_students--;
    pthread_exit(NULL);
}

int workflow_manager(int total_students,int max_students){
    int i ;
    pthread_t threads[total_students]; // Array to store thread IDs 
    reading_room = create_room(max_students, total_students);

    char** students = init_students(total_students);

    for (i = 0; i < total_students; i++)
    {
        if (pthread_create(&threads[i], NULL, thread_function,students[i]) != 0)
        {
            perror("Thread creation failed");
            return 1;
        }
    }

    // Wait for all threads to finish
    for (i = 0; i < total_students; i++)
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
    workflow_manager(20,8); //TODO: add cmd args
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

void print_room(room *room){

    // Print study room status
    printf("Study room: |");
    for (int i = 0; i < room->total_students; i++) {
        if (room->students[i]->state == STUDYING) {
            printf(" %s |", room->students[i]->AM);
        }
    }
    printf("\n");

    // Print waiting room status
    printf("Waiting: |");
    for (int i = 0; i < room->total_students; i++) {
        if (room->students[i]->state == WAITING) {
            printf(" %s |", room->students[i]->AM);
        }
    }
    printf("\n");
}

room *create_room(int max_students, int total_students) {
    room *new_room = (room *)malloc(sizeof(room));

    new_room->max_students = max_students;
    new_room->curr_students = 0;
    new_room->total_students = total_students;
    new_room->is_full = 0;

    new_room->students = (student **)malloc(max_students * sizeof(student *));

    sem_init(&new_room->sem, 0,8); 
    sem_init(&new_room->full_sem, 0,1);

    return new_room;
}