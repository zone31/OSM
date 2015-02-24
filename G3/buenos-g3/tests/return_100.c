#include "lib.h"
#include "proc/syscall.h"

int main()
{
;

    syscall_write(FILEHANDLE_STDOUT, "return100.c of 100\n", 100);



    return 100;
}
