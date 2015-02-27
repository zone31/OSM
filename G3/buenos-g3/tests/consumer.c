#include "tests/lib.h"
#include "proc/semaphore.h"

int main(){
    usr_sem_t *semHandle = syscall_sem_open("coolSemaphore",-1);
    syscall_sem_p(semHandle);
    syscall_sem_v(semHandle);
    syscall_exit(0);
    return 0;
}
