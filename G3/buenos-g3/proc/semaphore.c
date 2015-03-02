#include "kernel/semaphore.h"
#include "semaphore.h"
#include "kernel/interrupt.h"
#include "kernel/sleepq.h"
#include "kernel/assert.h"

/* Helper functions. */
usr_sem_t_wrapper * get_semaphore(char const *name);
usr_sem_t_wrapper * alloc_semaphore(char const *name);
usr_sem_t_wrapper * create_semaphore(char const *name, int value);

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

/* Creates a new semaphore if the value is positive and returns an existing
 * semaphore if the value is negative.  The function returns NULL if an error
 * occurred. */
usr_sem_t * syscall_sem_open(char const *name, int value)
{
    if (value >= 0) {
        return &(create_semaphore(name, value)->ident);
    } else {
        return &(get_semaphore(name)->ident);
    }
}

/* Creates a semaphore with the given name and value. */
usr_sem_t_wrapper * create_semaphore(char const *name, int value)
{
    usr_sem_t_wrapper *new_sem;

    if ((new_sem = alloc_semaphore(name)) != NULL) {
        new_sem->kernel_semaphore = semaphore_create(value);
        stringcopy(new_sem->name, name, SEM_NAME_MAX_LENGTH-1);
        new_sem->initial_value = value;

        if (new_sem->kernel_semaphore == NULL) {
            return NULL;
        } else {
            return new_sem;
        }
    } else {
        return NULL;
    }
}

/* Find a semaphore in the array of semaphores with the same name as the
 * parameter given to the function. */
usr_sem_t_wrapper * get_semaphore(char const *name)
{
    int i;

    for (i = 0; i < SEM_MAX_SEMAPHORES; i++){
        if (stringcmp(name, semaphores[i].name) == 0) {
            return &semaphores[i];
        }
    }
    return NULL;
}

/* Test if a semaphore with the given name already exist, if it does the
 * function returns NULL otherwise it searches for a free place in the array.
 * If one is found the function set this semaphore's status as SEM_ALLOCED and
 * returns a pointer to this semaphore. */
usr_sem_t_wrapper * alloc_semaphore(char const *name){
    int i;

    if (get_semaphore(name) == NULL){
        for (i = 0; i < SEM_MAX_SEMAPHORES; i++){
            if(semaphores[i].status == SEM_NONALLOCED){
                semaphores[i].status = SEM_ALLOCED;
                return &semaphores[i];
            }
        }
    }

    return NULL;
}

/* Removes a semaphore with a given handle, the handle is dereferenced to be an
 * integer corresponding to an index in the array of semaphores.  This index is
 * used to find a semaphore and remove it. */
int syscall_sem_destroy(usr_sem_t* handle)
{
    interrupt_status_t intr_status;

    usr_sem_t_wrapper *handle_wrapper = &semaphores[*((int *)handle)];

    /* Semaphore doesn't exist. */
    if (handle_wrapper->status == SEM_NONALLOCED) {
        return SEM_DOES_NOT_EXIST;
    }

    /* Lock when testing if the semaphore is ready to be removed. */
    intr_status = _interrupt_disable();
    spinlock_acquire(&handle_wrapper->slock);

    /* Test if the semaphore is done being used. */
    if (handle_wrapper->initial_value !=
        handle_wrapper->kernel_semaphore->value) {

        /* Release the lock. */
        spinlock_release(&handle_wrapper->slock);
        _interrupt_set_state(intr_status);

        return SEM_IN_USE;
    }

    handle_wrapper->status = SEM_NONALLOCED;

    /* Destroy kernel semaphore. */
    semaphore_destroy(handle_wrapper->kernel_semaphore);

    /* Release the lock. */
    spinlock_release(&handle_wrapper->slock);
    _interrupt_set_state(intr_status);

    return SUCCESS;
}

int syscall_sem_v(usr_sem_t* handle)
{
    int index;
    usr_sem_t_wrapper *handle_wrapper;

    /* Return error if handle is NULL. */
    KERNEL_ASSERT(handle != NULL);

    /* Get the index in the array. */
    index = *((int *) handle);

    /* Get the actual handle and vacate the resource. */
    handle_wrapper = &(semaphores[index]);

    if (handle_wrapper->status == SEM_NONALLOCED) {
        return SEM_DOES_NOT_EXIST;
    }

    semaphore_V(handle_wrapper->kernel_semaphore);

    return 0;
}

int syscall_sem_p(usr_sem_t* handle)
{
    int index;
    usr_sem_t_wrapper *handle_wrapper;

    /* Return error if handle is NULL. */
    KERNEL_ASSERT(handle != NULL);

    /* Get the index in the array. */
    index = *((int *) handle);

    /* Get the actual handle and procure a resource. */
    handle_wrapper = &(semaphores[index]);

    if (handle_wrapper->status == SEM_NONALLOCED) {
        return SEM_DOES_NOT_EXIST;
    }

    semaphore_P(handle_wrapper->kernel_semaphore);

    return 0;
}
