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
    int waiting_line; // 
    int is_full;
    student **students; // array of students in the room
    sem_t *sem_enter;
    pthread_mutex_t mutex;
}room;

/**
 * @brief thread function for the students 
 * 
 * @param arg  student object
 * @return void* 
 */
void *thread_function(void *arg);
/**
 * @brief workflow manager for the threads
 * 
 * @param total_students  total number of students
 * @param max_students  max number of students in the room
 * @param init_value  0 for AMs , 1 for numbers
 * @return int 
 */
int workflow_manager(int total_students,int max_students,int init_value);
/**
 * @brief Create a room object
 * 
 * @param max_students  max number of students in the room
 * @param total_students  total number of students
 * @return room* 
 */
room *create_room(int max_students, int total_students);
/**
 * @brief print room status
 * 
 * @param room  room object
 * @param msg  message to print in the beginning
 */
void print_room(room *room, char *msg);
/**
 * @brief get random number between min and max
 * 
 * @param min 
 * @param max 
 * @return int 
 */
int random_number(int min, int max);
/**
 * @brief init students as AMs
 * 
 * @param max_students  max number of students
 * @return char** 
 */
char** init_students(int max_students);
/**
 * @brief init students as numbers
 * 
 * @param max_students max number of students
 * @return char** 
 */
char** init_students_numbers(int max_students);
/**
 * @brief free room object
 * 
 * @param room 
 */
void free_room(room *room);

#endif /* threads_h */