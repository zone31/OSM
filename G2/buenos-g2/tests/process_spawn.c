#include "process_spawn.h"
#include "proc/process.h"

const char *read_write = "[disk]rw";

int main(int argc, char const *argv[])
{
    syscall_exec(read_write);

    return 0;
}
