#include "tests/lib.h"
#include "proc/syscall.h"

int main(void)
{
    char buffer[128];

    syscall_write(FILEHANDLE_STDOUT, "Hi, press a key!\n", 18);
    syscall_read(FILEHANDLE_STDIN, buffer, 127);
    syscall_write(FILEHANDLE_STDOUT, "Pressed button was, ", 21);
    syscall_write(FILEHANDLE_STDOUT, buffer, 128);
    syscall_write(FILEHANDLE_STDOUT, "\n", 1);

    syscall_halt();

    return 0;
}
