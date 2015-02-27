#include "proc/semaphore.h"
#include "proc/process.h"
#include "tests/lib.h"

int main(){
    int coolVar = 1337;
    usr_sem_t *semaphore = syscall_sem_open("coolSemaphore",4);
    process_id_t coolConsumer = syscall_exec("[disk]consumer");
    /* syscall_join(coolConsumer); */
    while(1){}
}
