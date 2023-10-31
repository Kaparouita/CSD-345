#ifndef threads_h
#define threads_h

#include <semaphore.h>

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
    int max_students; // max number of students in the room
    int curr_students; // current number of students in the room
    int total_students;// total number of students
    student **students; // array of students in the room
    sem_t sem;
    sem_t full_sem;
    int is_full;
}room;

room *create_room(int max_students, int total_students);
void print_room(room *room);

int random_number(int min, int max);
char** init_students(int max_students);

#endif /* threads_h */