#include "kernel/process_control.h"
#include "proc/process.h"

int syscall_exec(const char *filename)
{
    return process_spawn(filename);
}

void syscall_exit(int retval)
{
    process_finish(retval);
}
