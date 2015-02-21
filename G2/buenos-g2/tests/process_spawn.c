#include "process_spawn.h"
#include "proc/process.h"
#include "kernel/process_control.h"

const char *read_write = "[disk]rw";

int main()
{
    syscall_exec(read_write);

    return 0;
}
