#include "tests/lib.h"

int main(void)

{
    const char *filename = "[disk]Mark_is_fag";
    const char *masterrace = "mark is god";
    int size = 128;
    int ret_val;
    int filehandle;

    filehandle = ret_val = syscall_open(filename);
    ret_val = syscall_write(filehandle, masterrace, size);
    syscall_exit(filehandle);
    // syscall_open();
    //
    //
    // syscall_exec();
    //
    //
    // syscall_halt();

    return 0;
}
