#include "kernel/semaphore.h"
#include "semaphore.h"

/* An array where we store the userland semaphores. */
usr_sem_t_wrapper semaphores[SEM_MAX_SEMAPHORES];

void sem_user_init(void)
{
    int i;

    for (i = 0; i < SEM_MAX_SEMAPHORES; i++){
        semaphores[i].status = SEM_NONALLOCED;
        semaphores[i].name = "";
        semaphores[i].self = i;
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
        if ((new_sem = alloc_semaphore(name)) != NULL){
            new_sem->kernel_semaphore = semaphore_create(value);
            new_sem->name = name;
            return new_sem;
        }
        else{
            return NULL;
        }
    } else {
        return get_semaphore(name);
    }
}

usr_sem_t *get_semaphore(char const *name){
    int i;

    for (i = 0; i < SEM_MAX_SEMAPHORES; i++){
        if(stringcmp(name, semaphores[i].name) == 0){
            return &semaphores[i].self;
        }
    }
    return NULL;
}

usr_sem_t *alloc_semaphore(char const *name){
    int i;

    if (get_semaphore(name) == NULL){
        for (i = 0; i < SEM_MAX_SEMAPHORES; i++){
            if(semaphores[i].status == SEM_NONALLOCED){
                semaphores[i].status = SEM_ALLOCED;
                return (usr_sem_t *)&semaphores[i].self;
            }
        }
    }
    return NULL;
}

int syscall_sem_destroy(usr_sem_t* handle){
    usr_sem_t_wrapper *handle_wrapper = &semaphores[*((int *)handle)];
    handle_wrapper->status = SEM_NONALLOCED;
    return 0;
}

int syscall_sem_v(usr_sem_t* handle){
    if (handle == NULL){ return HANDLE_IS_NULL; }
    usr_sem_t_wrapper *handle_wrapper = &semaphores[*((int *)handle)];

    semaphore_V(handle_wrapper->kernel_semaphore);

    return 0;
}

int syscall_sem_p(usr_sem_t* handle){
    if (handle == NULL){ return HANDLE_IS_NULL; }

    usr_sem_t_wrapper *handle_wrapper = &semaphores[*((int *)handle)];
    semaphore_P(handle_wrapper->kernel_semaphore);
    return 0;
}
