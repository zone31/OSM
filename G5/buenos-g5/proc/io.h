/*
 * Buenos userland I/O support.
 * 
 * Copyright (C) 2015 OSM Course Team.
 * 
 * Sandstone Basic License - Version 0.2 - June 16, 2014.
 *
 * Permission is hereby granted, free of charge, to any legal person obtaining
 * these artefacts, to do with the artefacts as they please, including, without
 * limitation, the rights to use, copy, modify, distribute, sublicense, and/or
 * sell these artefacts, and to permit legal persons, to whom the artefacts are
 * furnished, to do so as well, subject to the following conditions: 
 *
 * The creators of these artefacts are properly attributed for their original
 * work, e.g. by including the above copyright notice and the below notice of
 * no warranty in any copy or derivation of the artefacts.
 *
 * THE ARTEFACTS ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHENTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE OR NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE CREATORS OF THE ARTEFACTS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES, OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF, OR IN CONNECTION WITH THE ARTEFACTS, THE USE, OR OTHER
 * DEALINGS IN THE ARTEFACTS.
 *
 */

#ifndef BUENOS_PROC_IO_H
#define BUENOS_PROC_IO_H

int io_read(int filehandle, void* buffer, int length);
int io_write(int filehandle, void* buffer, int length);

#endif /* BUENOS_PROC_IO_H */
