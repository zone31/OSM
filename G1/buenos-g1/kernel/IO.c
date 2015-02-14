#include "kernel/IO.h"
#include "lib/libc.h"
#include "drivers/gcd.h"
#include "lib/libc.h"
#include "proc/syscall.h"
#include "drivers/device.h"
#include "kernel/assert.h"
#include "drivers/yams.h"

int syscall_read(int fhandle, void *buffer, int length)
{
    device_t *dev;
    gcd_t *gcd;

    if (fhandle != FILEHANDLE_STDIN)
        return -1;

    dev = device_get(YAMS_TYPECODE_TTY, 0);
    KERNEL_ASSERT(dev != NULL);

    gcd = (gcd_t *) dev->generic_device;
    KERNEL_ASSERT(gcd != NULL);

    return gcd->read(gcd, buffer, length);
}

int syscall_write(int fhandle, void const* buffer, int length)
{
    device_t *dev;
    gcd_t *gcd;

    if (fhandle != FILEHANDLE_STDOUT)
        return -1;

    dev = device_get(YAMS_TYPECODE_TTY, 0);
    KERNEL_ASSERT(dev != NULL);

    gcd = (gcd_t *) dev->generic_device;
    KERNEL_ASSERT(gcd != NULL);

    return gcd->write(gcd, buffer, length);
}
