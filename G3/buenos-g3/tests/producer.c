#include "tests/lib.h"

int main(){
    int coolVar = 1337;
    coolVar = coolVar;
    /* printf("Our tid: %d", thread_get_current_thread()); */
    printf("Before\n\n");
    usr_sem_t *semaphore = syscall_sem_open("coolSemaphore",4);
    printf("After1\n");
    semaphore = semaphore;
    int coolConsumer = syscall_exec("[disk]consumer");
    printf("After\n");
    syscall_join(coolConsumer);
    printf("Even more after\n");
    while(1){}
    return 0;
}
