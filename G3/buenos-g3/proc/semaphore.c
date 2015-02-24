#include "kernel/semaphore.h"
#include "semaphore.h"

/* An array where we store the userland semaphores. */
user_sem_t semaphores[SEM_MAX_SEMAPHORES];

void sem_user_init(void)
{
    int i;

    for (i = 0; i < SEM_MAX_SEMAPHORES; i++)
        semaphores[i].sem_status = SEM_FREE;
}

// int hash(const char *str)
// {
//     int hash = 1234;
//
//     while (*str != '\0')
//         sum *= *str;
//
//     return sum % SEM_MAX_SEMAPHORES;
// }

user_sem_t * syscall_sem_open(char const *name, int value)
{
    user_sem_t *new_sem;

    if (value >= 0) {
        if ((new_sem = get_semaphore(name)) != NULL)
            new_sem->kernel_semaphore = semaphore_create(value);
            new_sem->sem_status = SEM_TAKEN;
            new_sem->name = name;

            return new_sem;
        else
            return user_sem_t->kernel_semaphore;

        /* Create semaphore with value.  If already exist return NULL. */
    } else {
        return NULL;
    }
}

user_sem_t * get_semaphore(char const *name){
  int i;

  // for (i = 0; i < SEM_MAX_SEMAPHORES; i++){
  //     if(strcmp(name, semaphores[i].name)){
  //       return strcmp(name, semaphores[i];
  //     }
  //   }
  return NULL;

}
