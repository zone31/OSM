#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#define SEM_NAME_MAX_LENGTH 256
#define SEM_MAX_SEMAPHORES 32

typedef enum {
    SEM_FREE = 0,
    SEM_TAKEN
}semstatus;

typedef struct {
    semaphore_t *kernel_semaphore;
    semstatus sem_status;
    const char *name;
} user_sem_t;



user_sem_t * syscall_sem_open(char const *name, int value);
int syscall_sem_p(user_sem_t *handle);
int syscall_sem_v(user_sem_t *handle);
int syscall_sem_destroy(user_sem_t *handle);
void sem_user_init(void);
user_sem_t * get_semaphore(char const *name);

#endif
