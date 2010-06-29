/*
 *          DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                  Version 2, December 2004
 *
 *  Copyright (C) 2004 Sam Hocevar
 *  14 rue de Plaisance, 75014 Paris, France
 *  Everyone is permitted to copy and distribute verbatim or modified
 *  copies of this license document, and changing it is allowed as long
 *  as the name is changed.
 *  DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 *
 *
 *  David Hagege <david.hagege@gmail.com>
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include "cross.h"

void *cross_mmap(void *addr, size_t length, int prot, int flags,
        int fd, off_t offset)
{
#if defined(_WIN32)
    HANDLE fm;
    HANDLE h = (HANDLE)_get_osfhandle(fd);

    (void)addr;
    (void)prot;
    (void)flags;
    (void)offset;
    if ((fm = CreateFileMapping(h, NULL, PAGE_READONLY, 0, 0, NULL)) == NULL) {
        fprintf(stderr, "Can't CreateFileMapping\n");
    }
    return MapViewOfFile(fm, FILE_MAP_READ, 0, 0, length);
#else
    return mmap(addr, length, prot, flags, fd, offset);
#endif /* _WIN32 */
}

int cross_munmap(void *addr, size_t length)
{
#if defined(_WIN32)
    (void)length;
    return UnmapViewOfFile(addr);
#else
    return munmap(addr, length);
#endif /* _WIN32 */
}
