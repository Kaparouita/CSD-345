#ifndef threads_h
#define threads_h

#include <semaphore.h>
#include <pthread.h>

int thread_counter = 0;

typedef enum {
    STUDYING,
    WAITING,
    FINISHED,
}status;

typedef struct student {
    char *AM;
    int study_time;
    status state; 
    int thread_id;
}student;

typedef struct room {
    int curr_students; // current number of students in the room
    int total_students;// total number of students
    student **students; // array of students in the room
    sem_t *sem_enter;
    pthread_mutex_t mutex;
    int is_full;
}room;

/**
 * @brief Create a room object
 * 
 * @param max_students  max number of students in the room
 * @param total_students  total number of students
 * @return room* 
 */
room *create_room(int max_students, int total_students);
void print_room(room *room, char *msg);
int random_number(int min, int max);
char** init_students(int max_students);
char** init_students_numbers(int max_students);

#endif /* threads_h */