#include <stdio.h>
#include "threads.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CONCURRENT_THREADS 8
#define MAX_STUDY_TIME 15
#define MIN_STUDY_TIME 5

room *reading_room ;
int thread_counter ;

void *thread_function(void *arg)
{
    char message[100];
    student *s = (student *)arg;

    // Enter

    if (s->thread_id != 0)
        sem_wait(&reading_room->sem_enter[s->thread_id-1]); // wait for previous student to enter
    
    pthread_mutex_lock(&reading_room->mutex);
    reading_room->students[reading_room->curr_students] = (student *)malloc(sizeof(student));
    reading_room->students[reading_room->curr_students] = s;
    reading_room->curr_students++;
    
    if (thread_counter >= MAX_CONCURRENT_THREADS) {
        sprintf(message,"Student %s was born\nStudent %s cannot enter the room because its full\n", s->AM,s->AM);
        print_room(reading_room,message);
        pthread_mutex_unlock(&reading_room->mutex);
        sem_post(&reading_room->sem_enter[s->thread_id]);
        reading_room->is_full = 1;

        while( (reading_room->is_full == 1 ) || (reading_room->waiting_line != s->thread_id)){
            sleep(1);
            if (thread_counter == MAX_CONCURRENT_THREADS) {
                reading_room->is_full = 1;
            }
        }
        pthread_mutex_lock(&reading_room->mutex);
        reading_room->waiting_line++;
        thread_counter++;
        s->state = STUDYING;
        sprintf(message, "Student %s entered the room\n", s->AM);
        print_room(reading_room,message);
        sem_post(&reading_room->sem_enter[s->thread_id]);
        pthread_mutex_unlock(&reading_room->mutex);
    }
    else{
        thread_counter++;
        reading_room->waiting_line++;
        printf("Student %s was born\n", s->AM);
        sem_post(&reading_room->sem_enter[s->thread_id]);
        s->state = STUDYING;
        print_room(reading_room,"");
        pthread_mutex_unlock(&reading_room->mutex);
    }

    // Study

    sleep(s->study_time);


    // Leave

    pthread_mutex_lock(&reading_room->mutex);
    s->state = FINISHED;
    thread_counter--;
    sprintf(message, "Student %s finished ,studying time : %d sec\n", s->AM,s->study_time);
    print_room(reading_room,message);
    pthread_mutex_unlock(&reading_room->mutex);

    if (thread_counter == 0) {
        reading_room->is_full = 0;
    }

    pthread_exit(NULL);
}

int workflow_manager(int total_students,int max_students,int init_value){
    int i ;
    pthread_t threads[total_students]; // Array to store thread IDs 
    reading_room = create_room(max_students, total_students);
    thread_counter = 0;
    char** students;

    if (init_value == 0)
        students = init_students_numbers(total_students);
    else
        students = init_students(total_students);

    for (i = 0; i < total_students; i++)
    {
        // Create a new student
        student *s = (student *)malloc(sizeof(student));
        s->AM =  students[i];
        s->state = WAITING;
        s->study_time = random_number(MIN_STUDY_TIME, MAX_STUDY_TIME);
        s->thread_id = i;

        // Start his thread
        if (pthread_create(&threads[i], NULL, thread_function,s) != 0)
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

    // Free room
    free_room(reading_room);
    printf("All students finished studying ,free everything! \n");
    return 0;
}


int main(int argc, char *argv[])
{
    int total_students;
    int init_value = 1;

    if (argc == 2 && strcmp(argv[1], "-n") == 0) {
        init_value = 0;
    } else if (argc == 2) {
        printf("Invalid option. Usage: %s [-n]\n", argv[0]);
        return 1;
    }
    printf("Enter the total number of students: ");
    if (scanf("%d", &total_students) != 1) {
        printf("Invalid input. Please enter an integer.\n");
        return 1;
    }
    if(total_students < 20 || total_students > 40){
        printf("Invalid input. Please enter a number between 20-40.\n");
        return 1;
    }

    workflow_manager(total_students,MAX_CONCURRENT_THREADS,init_value);
    return 0;
}



char** init_students(int max_students){
    char** student_data = (char**)malloc(max_students * sizeof(char*));
    int student_count = 0;

    FILE *file = fopen("student_data.csv", "r");
    if (file == NULL) {
        perror("Error opening file");
        return student_data;
    }

    while (student_count < max_students) {
        student_data[student_count] = (char *)malloc(10 * sizeof(char));
        if (fscanf(file, " %9[^,],", student_data[student_count]) == 1) {
            student_count++;
        } else 
            break; // EOF or error  
    }

    fclose(file);
    return student_data;
}

char** init_students_numbers(int max_students){
    char** student_data = (char**)malloc(max_students * sizeof(char*));
    for (int i = 0; i < max_students; i++)
    {
        student_data[i] = (char *)malloc(10 * sizeof(char)); 
        sprintf(student_data[i] , "%d",i);
    }
    return student_data;
}

int random_number(int min, int max)
{
    return min + (rand() % (max - min));
}

void print_room(room *room,char *message){
    printf("%s",message);
    // Print study room status
    printf("Study room: |");
    for (int i = 0; i < room->curr_students; i++) {
        if (room->students[i]->state == STUDYING) {
            printf(" %s |", room->students[i]->AM);
        }
    }
    printf("\n");

    // Print waiting room status
    printf("Waiting: |");
    for (int i = 0; i < room->curr_students; i++) {
        if (room->students[i]->state == WAITING) {
            printf(" %s |", room->students[i]->AM);
        }
    }
    printf("\n\n");
}

room *create_room(int max_students, int total_students) {
    room *new_room = (room *)malloc(sizeof(room));

    new_room->curr_students = 0;
    new_room->total_students = total_students;
    new_room->is_full = 0;
    new_room->waiting_line = 0;

    new_room->students = (student **)malloc(max_students * sizeof(student *));


    new_room->sem_enter = (sem_t *)malloc(total_students * sizeof(sem_t));
    for (int i = 0; i < total_students; i++)
    {
        sem_init(&new_room->sem_enter[i], 0,0);
    }
   

    pthread_mutex_init(&new_room->mutex, NULL);

    return new_room;
}

void free_room(room *room) {
    for (int i = 0; i < room->total_students; i++)
    {
        free(room->students[i]);
        sem_destroy(&room->sem_enter[i]);
    }
    pthread_mutex_destroy(&room->mutex);
    free(room->students);
    sem_destroy(room->sem_enter);
    free(room);
}