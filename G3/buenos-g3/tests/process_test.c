#include "lib.h"
#include "proc/syscall.h"
#include "proc/process.h"

const char *return_100 = "[disk]return_100";

void printres(int, char *);

int main()
{
    // int process;
    int res;

    syscall_write(FILEHANDLE_STDOUT, "Testing processes\n", 20);

    /* Spawn a process and join it. */
    // process = syscall_exec(return_100);

    // res = syscall_join(process);

    // printres(res >= 0, "Join with a child"); /* Expect no error. */

    /* Try and join with a (hopefully dead) process. */
    res = syscall_join(17);

    printres(res == PROCESS_ILLEGAL_JOIN, "Join with dead process");

    /* Try and join with a process out of bounds. */
    res = syscall_join(-100);

    printres(res == PROCESS_ILLEGAL_JOIN, "Join with process out of bounds");

    syscall_halt();

    return 0;
}

int number = 0;

void printres(int boolean, char *test)
{
    number++;

    if (boolean) {
        syscall_write(FILEHANDLE_STDOUT, test, 100);
        syscall_write(FILEHANDLE_STDOUT, "success\n", 9);
    } else {
        syscall_write(FILEHANDLE_STDOUT, test, 100);
        syscall_write(FILEHANDLE_STDOUT, "failed\n", 9);
    }
}
