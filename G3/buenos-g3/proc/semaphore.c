#include "kernel/semaphore.h"

/* An array where we store the userland semaphores. */
usr_sem_t semaphores[SEM_MAX_SEMAPHORES];

void sem_user_init(void)
{
    int i;

    for (i = 0; i < SEM_MAX_SEMAPHORES; i++)
        semaphores[i].sem_status = SEM_FREE;
}

int hash(const char *str)
{
    int hash = 1234;

    while (*str != '\0')
        sum *= *str;

    return sum % SEM_MAX_SEMAPHORES;
}

usr_sem_t * syscall_sem_open(char const *name, int value)
{
    usr_sem_t new_sem;

    if (value >= 0) {
        if ((usr_sem_t = get_semaphore(name)) != NULL)
            return semaphore_create(value);
        else
            return usr_sem_t->kernel_semaphore;

        /* Create semaphore with value.  If already exist return NULL. */
    } else {
        /* Returns an existing semaphore with the given name. */
    }
}
