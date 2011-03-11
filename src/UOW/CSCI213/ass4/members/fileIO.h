/* $Id: fileIO.h 2 2007-07-19 13:00:48Z pdezwart $ */

#ifndef __FILEIO_H
#define __FILEIO_H

int allocate_number(void);

int get(Member *member, int member_number);
int put(Member *member, int member_number);

#endif
