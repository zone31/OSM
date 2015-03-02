#include "proc/semaphore.h"
#include "tests/lib.h"
#include "proc/process.h"
#include "lib/libc.h"

/* Semaphore locks when printing. */
usr_sem_t *lock_print = NULL;

void safe_print(const char *print_diz);

char const *producer = "[disk]magnus_producer";
char const *consumer = "[disk]magnus_consumer";
char const *semaphore_name = "my_semaphore";
char const *semaphore_name_print = "print_semaphore";

int main()
{
    int producer_return;
    int consumer_return;
    process_id_t producer_pid;
    process_id_t consumer_pid;
    usr_sem_t *sem;

    /* Initialize lock. */
    lock_print = syscall_sem_open(semaphore_name_print, 1);

    printf("the pointer is %p\n", lock_print);

    /* Create semaphore to be shared by consumer and producer. */
    safe_print("Creating semaphore with name\n");
    if ((sem = syscall_sem_open(semaphore_name, 0)) == NULL)
        safe_print("Creation of semaphore failed\n");
    else
        safe_print("Creation of semaphore successful\n");

    /* Spawn the producer. */
    safe_print("Spawning producer\n");
    producer_pid = syscall_exec(producer);

    /* Spawn the consumer. */
    safe_print("Spawning consumer\n");
    consumer_pid = syscall_exec(consumer);

    /* Join with procure. */
    producer_return = syscall_join(producer_pid);

    /* Close the semaphore. */
    while (syscall_sem_destroy(sem) == SEM_IN_USE) {
        safe_print("Trying to destroy.\n");
    }

    /* Join with consume. */
    consumer_return = syscall_join(consumer_pid);

    if (producer_return == consumer_return)
        safe_print("Semaphore test successful\n");
    else
        safe_print("Semaphore test failed\n");

    safe_print("test finished - halting system \"gracefully\"\n");
    syscall_halt();

    return 0;
}

void safe_print(const char *str)
{
    syscall_sem_p(lock_print);

    printf(str);

    syscall_sem_v(lock_print);
}
