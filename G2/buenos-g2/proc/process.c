/*
 * Process startup.
 *
 * Copyright (C) 2003-2005 Juha Aatrokoski, Timo Lilja,
 *       Leena Salmela, Teemu Takanen, Aleksi Virtanen.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: process.c,v 1.11 2007/03/07 18:12:00 ttakanen Exp $
 *
 */
#include "proc/process.h"
#include "proc/elf.h"
#include "kernel/thread.h"
#include "kernel/assert.h"
#include "kernel/interrupt.h"
#include "kernel/config.h"
#include "kernel/sleepq.h"
#include "lib/debug.h"
#include "lib/libc.h"
#include "fs/vfs.h"
#include "drivers/yams.h"
#include "vm/vm.h"
#include "vm/pagepool.h"




#include "proc/process.h"
#include "proc/elf.h"
#include "kernel/thread.h"
#include "kernel/assert.h"
#include "kernel/interrupt.h"
#include "kernel/config.h"
#include "fs/vfs.h"
#include "drivers/yams.h"
#include "vm/vm.h"
#include "vm/pagepool.h"


/** @name Process startup
 *
 * This module contains facilities for managing userland processes.
 */

process_control_block_t process_table[PROCESS_MAX_PROCESSES];

/**
 * Starts one userland process. The thread calling this function will
 * be used to run the process and will therefore never return from
 * this function. This function asserts that no errors occur in
 * process startup (the executable file exists and is a valid ecoff
 * file, enough memory is available, file operations succeed...).
 * Therefore this function is not suitable to allow startup of
 * arbitrary processes.
 *
 * @executable The name of the executable to be run in the userland
 * process
 */
void process_start(process_id_t pid) {
    kprintf("Now in: Process_start #001\n");
    thread_table_t *my_entry;
    pagetable_t *pagetable;
    uint32_t phys_page;
    context_t user_context;
    uint32_t stack_bottom;
    elf_info_t elf;
    openfile_t file;
    // process_control_block_t control_block = process_table[pid];
    int i;

    interrupt_status_t intr_status;

    my_entry = thread_get_current_thread_entry();

    /* If the pagetable of this thread is not NULL, we are trying to
       run a userland process for a second time in the same thread.
       This is not possible. */
    KERNEL_ASSERT(my_entry->pagetable == NULL);

    pagetable = vm_create_pagetable(thread_get_current_thread());
    KERNEL_ASSERT(pagetable != NULL);

    intr_status = _interrupt_disable();
    my_entry->pagetable = pagetable;
    _interrupt_set_state(intr_status);

    kprintf("Now in: Process_start #002\n");
    kprintf("Now in: Process_start #024\n");
    kprintf("process_table[pid].executable");
    file = vfs_open(process_table[pid].executable);
    kprintf("Now in: Process_start #023\n");
    /* Make sure the file existed and was a valid ELF file */
    KERNEL_ASSERT(file >= 0);
    KERNEL_ASSERT(elf_parse_header(&elf, file));

    /* Trivial and naive sanity check for entry point: */
    KERNEL_ASSERT(elf.entry_point >= PAGE_SIZE);

    kprintf("Now in: Process_start #021\n");
    /* Calculate the number of pages needed by the whole process
       (including userland stack). Since we don't have proper tlb
       handling code, all these pages must fit into TLB. */
    KERNEL_ASSERT(elf.ro_pages + elf.rw_pages + CONFIG_USERLAND_STACK_SIZE
          <= _tlb_get_maxindex() + 1);

    kprintf("Now in: Process_start #022\n");
    /* Allocate and map stack */
    for(i = 0; i < CONFIG_USERLAND_STACK_SIZE; i++) {
        phys_page = pagepool_get_phys_page();
        KERNEL_ASSERT(phys_page != 0);
        vm_map(my_entry->pagetable, phys_page,
               (USERLAND_STACK_TOP & PAGE_SIZE_MASK) - i*PAGE_SIZE, 1);
    }

    kprintf("Now in: Process_start #003\n");
    /* Allocate and map pages for the segments. We assume that
       segments begin at page boundary. (The linker script in tests
       directory creates this kind of segments) */
    for(i = 0; i < (int)elf.ro_pages; i++) {
        phys_page = pagepool_get_phys_page();
        KERNEL_ASSERT(phys_page != 0);
        vm_map(my_entry->pagetable, phys_page,
               elf.ro_vaddr + i*PAGE_SIZE, 1);
    }

    for(i = 0; i < (int)elf.rw_pages; i++) {
        phys_page = pagepool_get_phys_page();
        KERNEL_ASSERT(phys_page != 0);
        vm_map(my_entry->pagetable, phys_page,
               elf.rw_vaddr + i*PAGE_SIZE, 1);
    }

    /* Put the mapped pages into TLB. Here we again assume that the
       pages fit into the TLB. After writing proper TLB exception
       handling this call should be skipped. */
    intr_status = _interrupt_disable();
    tlb_fill(my_entry->pagetable);
    _interrupt_set_state(intr_status);

    kprintf("Now in: Process_start #004\n");
    /* Now we may use the virtual addresses of the segments. */

    /* Zero the pages. */
    memoryset((void *)elf.ro_vaddr, 0, elf.ro_pages*PAGE_SIZE);
    memoryset((void *)elf.rw_vaddr, 0, elf.rw_pages*PAGE_SIZE);

    stack_bottom = (USERLAND_STACK_TOP & PAGE_SIZE_MASK) -
        (CONFIG_USERLAND_STACK_SIZE-1)*PAGE_SIZE;
    memoryset((void *)stack_bottom, 0, CONFIG_USERLAND_STACK_SIZE*PAGE_SIZE);

    /* Copy segments */

    if (elf.ro_size > 0) {
    /* Make sure that the segment is in proper place. */
        KERNEL_ASSERT(elf.ro_vaddr >= PAGE_SIZE);
        KERNEL_ASSERT(vfs_seek(file, elf.ro_location) == VFS_OK);
        KERNEL_ASSERT(vfs_read(file, (void *)elf.ro_vaddr, elf.ro_size)
              == (int)elf.ro_size);
    }

    if (elf.rw_size > 0) {
    /* Make sure that the segment is in proper place. */
        KERNEL_ASSERT(elf.rw_vaddr >= PAGE_SIZE);
        KERNEL_ASSERT(vfs_seek(file, elf.rw_location) == VFS_OK);
        KERNEL_ASSERT(vfs_read(file, (void *)elf.rw_vaddr, elf.rw_size)
              == (int)elf.rw_size);
    }

    kprintf("Now in: Process_start #005");

    /* Set the dirty bit to zero (read-only) on read-only pages. */
    for(i = 0; i < (int)elf.ro_pages; i++) {
        vm_set_dirty(my_entry->pagetable, elf.ro_vaddr + i*PAGE_SIZE, 0);
    }

    /* Insert page mappings again to TLB to take read-only bits into use */
    intr_status = _interrupt_disable();
    tlb_fill(my_entry->pagetable);
    _interrupt_set_state(intr_status);

    kprintf("Now in: Process_start #006");
    /* Initialize the user context. (Status register is handled by
       thread_goto_userland) */
    memoryset(&user_context, 0, sizeof(user_context));
    user_context.cpu_regs[MIPS_REGISTER_SP] = USERLAND_STACK_TOP;
    user_context.pc = elf.entry_point;

    process_table[pid].process_state = PROCESS_RUNNING;
    // control_block.process_state = PROCESS_RUNNING;

    kprintf("process is running\n");

    thread_goto_userland(&user_context);

    KERNEL_PANIC("thread_goto_userland failed.");
}

/* Initialize all processes to be PROCESS_DEAD. */
void process_init()
{
    int i;

    for (i = 0; i < PROCESS_MAX_PROCESSES; i++)
        process_table[i].process_state = PROCESS_DEAD;
}

process_id_t process_spawn(const char *executable) {
    process_id_t pid;
    process_control_block_t control_block;
    TID_t thread;

    /* Test if there is space for another process in the process_table. */
    pid = alloc_process();

    /* REPORT: we throw a kernel panic, if process table is full. */
    if (pid == PROCESS_MAX_PROCESSES)
        KERNEL_PANIC("Not space for any more processes.");

    stringcopy(control_block.executable, executable, PROCESS_MAX_FILELENGTH);
    control_block.pid = pid;
    control_block.process_state = PROCESS_INIT;

    /* Set the parent of the new process as the process running currently. */
    control_block.parent = process_get_current_process();

    /* Insert the process in the array of processes. */
    process_table[control_block.pid] = control_block;

    /* Start a new thread and start the process in it. */
    thread = thread_create((void (*)(uint32_t))(&process_start), pid);
    thread_run(thread);

    kprintf("process is init\n");

    return control_block.pid;
}

/* The function returns a process_id_t if there is space for another process in
 * process_table.  If there is not space, the function returns
 * PROCESS_PTABLE_FULL. */
process_id_t alloc_process(void) {
    int i;

    for (i = 0; i < PROCESS_MAX_PROCESSES; i++)
        if (process_table[i].process_state == PROCESS_DEAD)
            return i;

    return PROCESS_PTABLE_FULL;
}

/* Stop the process and the thread it runs in.  Sets the return value as
   well. */
void process_finish(int retval)
{
    process_id_t pid = process_get_current_process();
    thread_table_t *thread = thread_get_current_thread_entry();

    kprintf("process finish running\n");

    process_table[pid].process_state = PROCESS_ZOMBIE;
    process_table[pid].exit_code = retval;

    vm_destroy_pagetable(thread->pagetable);
    thread->pagetable = NULL;

    kprintf("process lololo end\n");
    thread_finish();

    kprintf("process finish end\n");
}

spinlock_t process_table_slock;
int process_join(process_id_t pid)
{
    int return_value;
    interrupt_status_t intr_status;
    intr_status = _interrupt_disable();

    spinlock_acquire(&process_table_slock);
    /* The process trying to join. */
    process_id_t parent = process_get_current_process();


    kprintf("process.c parent %d child %d 01\n", parent, pid);
    kprintf("process.c child's parent: %d 02\n", process_table[pid].parent);

    /* Test if pid is legal. */
    if (pid < 0 || pid >= PROCESS_MAX_PROCESSES) {
        KERNEL_PANIC("process illegal join");
        return PROCESS_ILLEGAL_JOIN;
    }

    /* Test if it is the parent trying to join, if not it is an illegal join. */
    if (process_table[pid].parent != parent)
        return PROCESS_ILLEGAL_JOIN;
    // KERNEL_ASSERT(process_table[pid].parent == parent);

    kprintf("waiting for child to become zombi\n");
    /* Wait for the process to call process_finish and become a zombie. */
    while (process_table[pid].process_state != PROCESS_ZOMBIE) {
        spinlock_release(&process_table_slock);
        sleepq_add(&process_table[pid]);
        thread_switch();
        spinlock_acquire(&process_table_slock);
        kprintf("%d", process_table[pid].process_state);
    }
    kprintf("\n");

    kprintf("after child became zombi\n");

    return_value = process_table[pid].exit_code;
    process_set_dead(pid); /* Kill the process. */


    spinlock_release(&process_table_slock);
    _interrupt_set_state(intr_status);

    return return_value;
}

void process_set_dead(process_id_t pid)
{
    process_table[pid].process_state = PROCESS_DEAD;
}

process_id_t process_get_current_process(void) {
    return thread_get_current_thread_entry()->process_id;
}

process_control_block_t *process_get_current_process_entry(void) {
    return &process_table[process_get_current_process()];
}

process_control_block_t *process_get_process_entry(process_id_t pid) {
    return &process_table[pid];
}

/** @} */
