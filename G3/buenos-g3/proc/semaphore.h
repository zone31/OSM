#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "kernel/semaphore.h"

#define SEM_NAME_MAX_LENGTH 256
#define SEM_MAX_SEMAPHORES 32

typedef enum {
    SEM_NONALLOCED = 0,
    SEM_ALLOCED
}semstatus;

typedef struct {
    semaphore_t *kernel_semaphore;
    semstatus status;
    const char *name;
    int self;
} usr_sem_t_wrapper;

typedef void usr_sem_t;

#define HANDLE_IS_NULL -1
#define SUCCESS 0


usr_sem_t * syscall_sem_open(char const *name, int value);
int syscall_sem_p(usr_sem_t *handle);
int syscall_sem_v(usr_sem_t *handle);
int syscall_sem_destroy(usr_sem_t *handle);
void sem_user_init(void);
usr_sem_t * get_semaphore(char const *name);
usr_sem_t *alloc_semaphore(char const *name);

#endif
