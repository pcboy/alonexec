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

#include <sys/stat.h>
#include <sys/mman.h>
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
#endif

#include "utils.h"

const char *getTempDirectory(void)
{
    static char *tmp = NULL;
    static char tmpfold[2048] = {0};

    if (tmp)
        return tmp;
    if (!(tmp = getenv("TMPDIR"))) {
        tmp = P_tmpdir;
    }
    snprintf(tmpfold, sizeof(tmpfold), "%s/alonexectmp.XXXXXX", tmp);
    if (!(tmp = mkdtemp(tmpfold))) {
        perror("mkdtemp");
        fprintf(stderr, "Can't create temporary directory %s\n", tmpfold);
        exit(EXIT_FAILURE);
    }
    return tmp;
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
    int fd;

    if ((siz = getFileSize(file)) < 0) {
        return NULL;
    }
    res = malloc(sizeof(file_t));
    res->len = siz;
    if ((fd = open(file, O_RDONLY, 0)) < 0) {
        perror("open");
        fprintf(stderr, "Can't open %s\n", file);
        free(res);
        return NULL;
    }
    res->data = mmap(NULL, sizeof(char) * res->len,
            PROT_READ, MAP_PRIVATE, fd, 0);
    if (res->data == MAP_FAILED) {
        perror("mmap");
        fprintf(stderr, "Can't mmap() %s.\n", file);
        free(res);
        return NULL;
    }
    close(fd);
    return res;
}

int closeFile(file_t *f)
{
    int res;

    if ((res = munmap(f->data, f->len)) == EINVAL)
        perror("munmap");
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
