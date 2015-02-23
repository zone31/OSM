#include "tests/npj.h"
#include "tests/lib.h"
#include "proc/syscall.h"

const char *return_100 = "[disk]return_100";

void reverse(char *string, int len)
{
    char tmp;
    char *end = string + len - 1;

    while (end > string) {
        tmp = *end;
        *end = *string;
        *string = tmp;

        end--, string++;
    }
}

void itoa(int n, char s[])
{
    int i;

    i = 0;
    do {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    s[i] = '\0';

    reverse(s, i);
}

int main()
{
    syscall_write(FILEHANDLE_STDOUT, "NU Korer det!!!\n", 100);

    // process_table[0].process_state = 5;

    syscall_exit(0);

    return 0;
















    // char char_sum[100];
    // char *c = char_sum;
    // int sum = 0;
    // int p1 = syscall_exec(return_100);
    // // int p2 = syscall_exec(return_100);
    // // int p3 = syscall_exec(return_100);
    // // int p4 = syscall_exec(return_100);
    // // int p5 = syscall_exec(return_100);
    // // int p6 = syscall_exec(return_100);

    // syscall_write(FILEHANDLE_STDOUT, "not_process_join 01\n", 100);

    // /* Join all the processes. */
    // sum += syscall_join(p1);
    // // sum += syscall_join(p2);
    // // sum += syscall_join(p3);
    // // sum += syscall_join(p4);
    // // sum += syscall_join(p5);
    // // sum += syscall_join(p6);

    // syscall_write(FILEHANDLE_STDOUT, "not_process_join 02\n", 100);

    // itoa(sum, char_sum);
    // while (*c++ != '\0')
    //     ;

    // *c++ = '\n';
    // *c++ = '\0';

    // syscall_write(FILEHANDLE_STDOUT, "not_process_join 03\n", 100);

    // // syscall_exit(0);

    // return 0;
}
