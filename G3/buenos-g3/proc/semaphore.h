#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "kernel/semaphore.h"
#include "kernel/spinlock.h"

#define SEM_NAME_MAX_LENGTH 256
#define SEM_MAX_SEMAPHORES  32
#define HANDLE_IS_NULL      -1
#define SUCCESS             0
#define SEM_DOES_NOT_EXIST  -2
#define SEM_IN_USE          -3

typedef enum {
    SEM_NONALLOCED = 0,
    SEM_ALLOCED
} semstatus;

typedef struct {
    semaphore_t *kernel_semaphore; /* A pointer to the kernel impl. */
    semstatus status; /* The status of the semaphore. */
    char name[SEM_NAME_MAX_LENGTH]; /* The name of the semaphore. */
    int ident; /* The position in the array of semaphores. */
    int initial_value;
    spinlock_t slock;
} usr_sem_t_wrapper;

typedef void usr_sem_t;

usr_sem_t * syscall_sem_open(char const *name, int value);
int syscall_sem_p(usr_sem_t *handle);
int syscall_sem_v(usr_sem_t *handle);
int syscall_sem_destroy(usr_sem_t *handle);
void sem_user_init(void);

#endif
