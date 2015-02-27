#include "proc/semaphore.h"

int main(){
    usr_sem_t *semHandle = get_semaphore("coolSemaphore");
    syscall_sem_p(semHandle);
    syscall_sem_v(semHandle);
}
