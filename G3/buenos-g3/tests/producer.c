#include "tests/lib.h"
#include "proc/semaphore.h"
#include "proc/process.h"

int main(){
    int coolVar = 1337;
    coolVar = coolVar;
    usr_sem_t *semaphore = syscall_sem_open("coolSemaphore",4);
    semaphore = semaphore;
    process_id_t coolConsumer = syscall_exec("[disk]consumer");
    syscall_join(coolConsumer);
    while(1){}
    return 0;
}
