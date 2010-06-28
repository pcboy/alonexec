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

#if defined(_WIN32)
#include <io.h>
#else
#include <sys/mman.h>
#endif /* _WIN32 */

#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#ifdef _WIN32
#include <sys/types.h>
#include <malloc.h>
#define lstat(x,y) _stat(x,y)
#define mkdir(x,y) mkdir(x)
#endif

#include "cross.h"
#include "utils.h"

const char *getTempDirectory(int id)
{
    static char *tmp = NULL;
    static char tmpf[2048] = {0};

    if (tmp)
        return tmp;
    if (!(tmp = getenv("TMPDIR"))) {
#ifndef _WIN32
        tmp = P_tmpdir;
#else
        tmp = alloca(1024);
        GetCurrentDirectory(1024, tmp);
#endif
    }
    snprintf(tmpf, sizeof(tmpf), "%s%calonexectmp.%i",
            tmp, CROSS_SLASH, id);
    mkdir(tmpf, 0755);
    return tmpf;
}

int notQuote(int c)
{
    return c != '"';
}

char *removeChars(char *line, int (*keepEq)(int))
{
    char *res; 
    int i, h = 0;

    if (!line)
        return NULL;
    res = alloca(strlen(line)+1);
    for (i = 0; line[i]; ++i) {
        if (keepEq(line[i])) { /* FIXME: Will causes problem in the future.
                                  What to do for 8c,9c binary names for instance ?*/
            res[h++] = line[i];
        }
    }
    res[h] = '\0';
    return h ? strdup(res) : NULL;
}

ssize_t getFileSize(char *file)
{
    struct stat s;

    if (lstat(file, &s) < 0) {
        perror("lstat");
        fprintf(stderr, "Can't lstat %s\n", file);
        return -1;
    }
    return s.st_size;
}

file_t *getFileContents(char *file)
{
    ssize_t siz;
    file_t *res;

    if ((siz = getFileSize(file)) < 0) {
        return NULL;
    }
    res = malloc(sizeof(file_t));
    res->len = siz;
    if ((res->fd = open(file, O_RDONLY, 0)) < 0) {
        perror("open");
        fprintf(stderr, "Can't open %s\n", file);
        free(res);
        return NULL;
    }
    res->data = cross_mmap(NULL, sizeof(char) * res->len,
            PROT_READ, MAP_PRIVATE, res->fd, 0);

    if (res->data == (void*)MAP_FAILED) {
        perror("mmap");
        fprintf(stderr, "Can't mmap() %s.\n", file);
        free(res);
        close(res->fd);
        return NULL;
    }
    return res;
}

int closeFile(file_t *f)
{
    int res;

    res = cross_munmap(f->data, f->len);
    close(f->fd);
    free(f), f = NULL;
    return res;
}

unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;

    if (!str)
        return 0;
    while(*str!='\0') {
        int c = *str;
        /* hash = hash*33 + c */
        hash = ((hash << 5) + hash) + c;
        str++;
    }
    return hash;
}

char *lowercase(char *str)
{
    char *p = str;

    if (!str)
        return NULL;
    while (*p++) {
        *p = tolower(*p);
    }
    return str;
}
