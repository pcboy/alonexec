#ifndef _CROSS_H_
#define _CROSS_H_

#if defined(_WIN32)
#include <windows.h>
#include <io.h>
#define stat _stat

#define PROT_READ FILE_MAP_READ
#define MAP_PRIVATE 0
#define MAP_FAILED -1
#define CROSS_SLASH '\\'
#else
#include <sys/mman.h>
#define CROSS_SLASH '/'
#endif /* _WIN32 */

char *cross_getAppDir(void);
char *cross_getAppPath(void);
void *cross_mmap(void *addr, size_t length, int prot, int flags,
                int fd, off_t offset);
int cross_munmap(void *addr, size_t length);

#endif /* _CROSS_H_ */
