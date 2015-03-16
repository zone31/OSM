#include "tests/lib.h"

int main()
{
    const char *filename = "[disk]Mark_is_fag";
    const char *write_this = "mark is fag";
    const char *write_this_also = "Mads is fag";
    const char *masterrace = "magnus is god";
    int size = 128;
    int ret_val;
    int filehandle;
    char read_buf[129];

    /* Create a file. */
    ret_val = syscall_create(filename, size);

    printf("create returned %d\n", ret_val);

    /* Open the file. */
    filehandle = ret_val = syscall_open(filename);

    printf("open returned %d\n", ret_val);

    int i;
    for (i = 0; i<127;i++){
        ret_val = syscall_write(filehandle,(void *) "#",1);
    }
    syscall_seek(filehandle,0);

    /* Write to the file. */
    ret_val = syscall_write(filehandle, (void *) write_this, 11);

    printf("write returned %d\n", ret_val);

    /* Change position in file. */
    ret_val = syscall_seek(filehandle, 50);

    printf("seek returned %d\n", ret_val);

    /* Write again. */
    ret_val = syscall_write(filehandle, (void *) write_this_also, 11);

    /* Change position again. */
    ret_val = syscall_seek(filehandle, 20);

    /* Write to file. */
    ret_val = syscall_write(filehandle, (void *) masterrace, 13);

    printf("tell: %d\n",syscall_tell(filehandle));

    /* Read the file. */
    ret_val = syscall_seek(filehandle, 0);
    ret_val = syscall_read(filehandle, read_buf, 128);
    printf("ret_val %d\n",ret_val);

    printf("tell: %d\n",syscall_tell(filehandle));
    printf("read %s\n", read_buf);

    /* Close the filehandle. */
    printf("tell: %d\n",syscall_tell(filehandle));
    ret_val = syscall_close(filehandle);

    printf("close returned %d\n", ret_val);

    /* Remove file. */
    ret_val = syscall_delete(filename);

    syscall_exit(0);

    return 0;
}
