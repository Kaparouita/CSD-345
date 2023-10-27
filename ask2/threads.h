#ifndef threads_h
#define threads_h


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


int random_number(int min, int max);
char** init_students(int max_students);

#endif /* threads_h */