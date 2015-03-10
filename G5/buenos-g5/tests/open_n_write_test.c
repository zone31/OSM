#include "tests/lib.h"

int main()
{
    const char *filename = "[disk]Mark_is_fag";
    int size = 1 << 3;
    int ret_val;
    int filehandle;

    /* Create a file. */
    ret_val = syscall_create(filename, size);

    printf("create returned %d\n", ret_val);

    /* Open the file. */
    filehandle = ret_val = syscall_open(filename);

    printf("open returned %d\n", ret_val);

    /* Write to the file. */
    ret_val = syscall_write(filehandle, (void *) "mark is fag", 100);

    printf("write returned %d\n", ret_val);

    /* Close the filehandle. */
    ret_val = syscall_close(filehandle);

    printf("close returned %d\n", ret_val);

    syscall_exit(0);

    return 0;
}
