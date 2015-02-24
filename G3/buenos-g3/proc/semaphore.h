#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#define SEM_NAME_MAX_LENGTH 100
#define SEM_MAX_SEMAPHORES 32

enum semstatus {
    SEM_FREE,
    SEM_TAKEN
}

typedef struct {
    semaphore_t *kernel_semaphore;
    semstatus sem_status;
} usr_sem_t;

usr_sem_t * syscall_sem_open(char const *name, int value);
int syscall_sem_p(user_sem_t *handle);
int syscall_sem_v(user_sem_t *handle);
int syscall_sem_destroy(user_sem_t *handle);
void sem_user_init(void);
usr_sem_t * get_semaphore(char const *name);

#endif
