#ifndef PROCESS_CONTROL_H
#define PROCESS_CONTROL_H

int syscall_exec(const char *filename);
void syscall_exit(int retval);
int syscall_join(int pid);

#endif
