#ifndef IO_H
#define IO_H

int syscall_read(int fhandle, void *buffer, int length);
int syscall_write(int fhandle, void const *buffer, int length);

#endif
