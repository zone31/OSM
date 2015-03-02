#include "kernel/semaphore.h"
#include "semaphore.h"
#include "kernel/kmalloc.h" /* REMOVE. */
#include "kernel/assert.h"  /* REMOVE. */

/* Helper functions. */
usr_sem_t_wrapper * get_semaphore(char const *name);
usr_sem_t_wrapper *alloc_semaphore(char const *name);

/* An array where we store the userland semaphores. */
usr_sem_t_wrapper semaphores[SEM_MAX_SEMAPHORES];

void sem_user_init(void)
{
    int i;

    for (i = 0; i < SEM_MAX_SEMAPHORES; i++){
        semaphores[i].status = SEM_NONALLOCED;
        semaphores[i].name[0] = '\0';
        semaphores[i].ident = i;
    }
}

// int hash(const char *str)
// {
//     int hash = 1234;

//     while (*str != '\0')
//         hash *= *str++;

//     return hash % SEM_MAX_SEMAPHORES;
// }

usr_sem_t * syscall_sem_open(char const *name, int value)
{
    usr_sem_t_wrapper *new_sem;

    if (value >= 0) {
        /* Create semaphore with value.  If already exist return NULL. */
        if ((new_sem = alloc_semaphore(name)) != NULL) {
            new_sem->kernel_semaphore = semaphore_create(value);
            stringcopy(new_sem->name, name, SEM_NAME_MAX_LENGTH-1);

            if (new_sem->kernel_semaphore == NULL) {
                return NULL;
            } else {
                return &(new_sem->ident);
            }
        } else {
            return NULL;
        }
    } else {
        return get_semaphore(name);
    }
}

usr_sem_t_wrapper *get_semaphore(char const *name){
    int i;

    for (i = 0; i < SEM_MAX_SEMAPHORES; i++){
        if(stringcmp(name, semaphores[i].name) == 0){
            return &semaphores[i];
        }
    }
    return NULL;
}

usr_sem_t_wrapper *alloc_semaphore(char const *name){
    int i;

    if (get_semaphore(name) == NULL){
        for (i = 0; i < SEM_MAX_SEMAPHORES; i++){
            if(semaphores[i].status == SEM_NONALLOCED){
                semaphores[i].status = SEM_ALLOCED;
                return (usr_sem_t *)&semaphores[i];
            }
        }
    }
    return NULL;
}

int syscall_sem_destroy(usr_sem_t* handle){
    usr_sem_t_wrapper *handle_wrapper = &semaphores[*((int *)handle)];
    handle_wrapper->status = SEM_NONALLOCED;

    /* Destroy kernel semaphore. */
    semaphore_destroy(handle_wrapper->kernel_semaphore);

    return 0;
}

int syscall_sem_v(usr_sem_t* handle){
    int index;
    usr_sem_t_wrapper *handle_wrapper;

    /* Return error if handle is NULL. */
    if (handle == NULL) {
        return HANDLE_IS_NULL;
    }

    /* Get the index in the array. */
    index = *((int *)handle);

    /* Get the actual handle and vacate the resource. */
    handle_wrapper = &(semaphores[index]);
    semaphore_V(handle_wrapper->kernel_semaphore);

    return 0;
}

int syscall_sem_p(usr_sem_t* handle){
    int index;
    usr_sem_t_wrapper *handle_wrapper;

    /* Return error if handle is NULL. */
    if (handle == NULL) { return HANDLE_IS_NULL; }

    /* Get the index in the array. */
    index = *((int *) handle);

    /* Get the actual handle and procure a resource. */
    handle_wrapper = &(semaphores[index]);
    semaphore_P(handle_wrapper->kernel_semaphore);

    return 0;
}
