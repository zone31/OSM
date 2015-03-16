#include "tests/lib.h"

int main(void)
{
    const char *filename = "[disk]Mark_is_fag";
    const char *masterrace = "magnus is god";
    int size = 128;
    int ret_val;
    int filehandle;
    char read_buf[129];




    /* Create a file. */
    ret_val = syscall_create(filename, size);


    filehandle = ret_val = syscall_open(filename);

    syscall_write(filehandle, masterrace, size);

//    printf("create returned %d\n", ret_val);

    ret_val = syscall_close(filehandle);
    int proc = syscall_exec("[disk]proc_file_help");
    printf("proc id %d\n",proc);
    filehandle = syscall_join(proc);


    ret_val = syscall_seek(filehandle,0);
    ret_val = syscall_read(filehandle,read_buf,size);

    printf("ret val  %d\n",ret_val);
    printf("readbuff value %s\n",read_buf);

    //syscall_halt();

    return 0;
}
