#include "lib.h"
#include "proc/syscall.h"

int main()
{
    // int i = 1 << 31;
    // int k = 0;

    syscall_write(FILEHANDLE_STDOUT, "return100.c Mads is faggot01\n", 100);

    // while (i--)
    //     k++;

    syscall_write(FILEHANDLE_STDOUT, "return100.c Mads is stadig faggot02\n", 100);

    // syscall_exit(100);

    // syscall_exit(100);

    return 100;
}
