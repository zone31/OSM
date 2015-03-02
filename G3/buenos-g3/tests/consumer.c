#include "tests/lib.h"
#include "proc/semaphore.h"

int main(){
    usr_sem_t *semHandle = syscall_sem_open("coolSemaphore",-1);
    printf("lulz");
    syscall_sem_p(semHandle);
    syscall_sem_v(semHandle);
    printf("not lulz");
    syscall_exit(0);
    return 0;
}
