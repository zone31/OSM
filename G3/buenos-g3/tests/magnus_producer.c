#include "proc/semaphore.h"
#include "tests/lib.h"
#include "lib/libc.h"

void safe_print(const char *print_diz_nigga);
usr_sem_t *lock_print = NULL;

char const *semaphore_name_print = "print_semaphore";
char const *semaphore_name = "my_semaphore";

int main()
{
    int i;
    int produced = 0; /* Number of produced resources. */
    int n = 10000;
    usr_sem_t *sem;

    /* Initialize lock. */
    lock_print = syscall_sem_open(semaphore_name_print, -1);

    /* Get the semaphore to produce resources. */
    safe_print("Producer trying to get the semaphore with the name\n");
    if ((sem = syscall_sem_open(semaphore_name, -1)) == NULL)
        safe_print("Semaphore not found\n");
    else
        safe_print("Semaphore found\n");

    /* Loop n times and procure that number of resources. */
    for (i = 0; i < n; i++) {
        syscall_sem_v(sem);
        produced++;
    }

    syscall_exit(produced);
    return 0;
}

void safe_print(const char *print_diz_nigga)
{
    syscall_sem_p(lock_print);

    printf(print_diz_nigga);

    syscall_sem_v(lock_print);
}
