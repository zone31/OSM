/*
 * TLB handling
 *
 * Copyright (C) 2003 Juha Aatrokoski, Timo Lilja,
 *   Leena Salmela, Teemu Takanen, Aleksi Virtanen.
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
 * $Id: tlb.c,v 1.6 2004/04/16 10:54:29 ttakanen Exp $
 *
 */

#include "kernel/panic.h"
#include "kernel/assert.h"
#include "vm/tlb.h"
#include "vm/pagetable.h"
#include "kernel/thread.h"
#include "vm/vm.h"

void tlb_modified_exception(void)
{
    tlb_exception_state_t state;
    thread_table_t *thread = thread_get_current_thread_entry();

    _tlb_get_exception_state(&state);

    /* Find out if we are in userland. */
    if (thread->user_context->status & USERLAND_ENABLE_BIT) {
        kwrite("Writing to write protected area\n");
        thread_finish();
    }

    KERNEL_PANIC("Kernel tlb modified exception");
}

void load_store(void)
{
    int i;
    tlb_exception_state_t state;
    tlb_entry_t *entry;
    thread_table_t *thread;
    int odd;
    int valid;
    int tlb_table_index;

    _tlb_get_exception_state(&state);
    kprintf("badvpn2: %d, badvaddr: %d, asid: %d\n",state.badvpn2,state.badvaddr,state.asid);
    thread = thread_get_current_thread_entry();

    // odd = ODD_VPN(state.badvaddr);
    odd = state.badvaddr & 4096;

    /* Loop through the pagetable and find a tlb_entry with the same vpn2. */
    for (i = 0; i < PAGETABLE_ENTRIES; i++) {
        kprintf("looking %d\n", i);
        entry = &thread->pagetable->entries[i];

        valid = odd ? entry->V0 : entry->V1;

        kprintf("valid %d\n", valid);

        kprintf("comparing VPN2 %d == badvpn2 %d, diff: %d\n", entry->VPN2, state.badvpn2,entry->VPN2-state.badvpn2);
        if (state.badvpn2)
            kprintf("not 0 statebadvpn2\n");
        if (entry->VPN2)
            kprintf("not 0 entry\n");

        if (entry->VPN2 == state.badvpn2 && valid) {
            kprintf("running the if\n");
            entry->ASID = state.asid;

            /* tlb_table_index is negative if there is no space in the tlb. */
            tlb_table_index = _tlb_probe(entry);

            if (tlb_table_index < 0) {
                _tlb_write_random(entry);
            } else {
                /* Write one pagetable. */
                _tlb_write(entry, tlb_table_index, 1);
            }

            return;
        }
    }

    /* Close the thread if userland and give kernel panic if the kernel is
     * generating the exception. */
    if (thread->user_context->status & USERLAND_ENABLE_BIT) {
        kwrite("Access violation\n");

        /* Remove the pagetable. */
        vm_destroy_pagetable(thread->pagetable);
        thread->pagetable = NULL;

        /* Close the thread. */
        thread_finish();
    } else {
        KERNEL_PANIC("Unhandled TLB load or store exception");
    }
}

void tlb_load_exception(void)
{
    load_store();
    // int i;
    // tlb_exception_state_t state;
    // tlb_entry_t *entry;
    // thread_table_t *thread;
    // int odd;
    // int valid;
    // int tlb_table_index;

    // _tlb_get_exception_state(&state);
    // thread = thread_get_current_thread_entry();

    // odd = ODD_VPN(state.badvpn2);

    // /* Loop through the pagetable and find a tlb_entry with the same vpn2. */
    // for (i = 0; i < PAGETABLE_ENTRIES; i++) {
    //     kprintf("looking\n");
    //     entry = &thread->pagetable->entries[i];

    //     valid = odd ? entry->V1 : entry->V0;

    //     if (entry->VPN2 == state.badvpn2 && valid) {
    //         entry->ASID = state.asid;

    //         /* tlb_table_index is negative if there is no space in the tlb. */
    //         tlb_table_index = _tlb_probe(entry);

    //         if (tlb_table_index < 0) {
    //             _tlb_write_random(entry);
    //         } else {
    //             /* Write one pagetable. */
    //             _tlb_write(entry, tlb_table_index, 1);
    //         }
    //     }
    // }

    // /* Close the thread if userland and give kernel panic if the kernel is
    //  * generating the exception. */
    // if (thread->user_context->status & USERLAND_ENABLE_BIT) {
    //     kwrite("Access violation\n");

    //     /* Remove the pagetable. */
    //     vm_destroy_pagetable(thread->pagetable);
    //     thread->pagetable = NULL;

    //     /* Close the thread. */
    //     thread_finish();
    // } else {
    //     KERNEL_PANIC("Unhandled TLB load exception");
    // }
}

void tlb_store_exception(void)
{
    load_store();
    // int i;
    // tlb_exception_state_t state;
    // tlb_entry_t *entry;
    // thread_table_t *thread;
    // int odd;
    // int valid;
    // int tlb_table_index;

    // _tlb_get_exception_state(&state);
    // thread = thread_get_current_thread_entry();

    // odd = ODD_VPN(state.badvpn2);

    // /* Loop through the pagetable and find a tlb_entry with the same vpn2. */
    // for (i = 0; i < PAGETABLE_ENTRIES; i++) {
    //     entry = &thread->pagetable->entries[i];

    //     valid = odd ? entry->V1 : entry->V0;

    //     if (entry->VPN2 == state.badvpn2 && valid) {
    //         entry->ASID = state.asid;

    //         /* tlb_table_index is negative if there is no space in the tlb. */
    //         tlb_table_index = _tlb_probe(entry);

    //         if (tlb_table_index < 0) {
    //             _tlb_write_random(entry);
    //         } else {
    //             /* Write one pagetable. */
    //             _tlb_write(entry, tlb_table_index, 1);
    //         }
    //     }
    // }

    // /* Close the thread if userland and give kernel panic if the kernel is
    //  * generating the exception. */
    // if (thread->user_context->status & USERLAND_ENABLE_BIT) {
    //     kwrite("Access violation\n");

    //     /* Remove the pagetable. */
    //     vm_destroy_pagetable(thread->pagetable);
    //     thread->pagetable = NULL;

    //     /* Close the thread. */
    //     thread_finish();
    // } else {
    //     KERNEL_PANIC("Unhandled TLB store exception");
    // }
}

/**
 * Fill TLB with given pagetable. This function is used to set memory
 * mappings in CP0's TLB before we have a proper TLB handling system.
 * This approach limits the maximum mapping size to 128kB.
 *
 * @param pagetable Mappings to write to TLB.
 *
 */

void tlb_fill(pagetable_t *pagetable)
{
    if(pagetable == NULL)
	return;

    /* Check that the pagetable can fit into TLB. This is needed until
     we have proper VM system, because the whole pagetable must fit
     into TLB. */
    KERNEL_ASSERT(pagetable->valid_count <= (_tlb_get_maxindex()+1));

    _tlb_write(pagetable->entries, 0, pagetable->valid_count);

    /* Set ASID field in Co-Processor 0 to match thread ID so that
       only entries with the ASID of the current thread will match in
       the TLB hardware. */
    _tlb_set_asid(pagetable->ASID);
}
