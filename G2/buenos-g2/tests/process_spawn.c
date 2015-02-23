#include "process_spawn.h"
#include "lib.h"

const char *read_write = "[disk]rw";

int main()
{
    syscall_exec(read_write);

    return 0;
}
