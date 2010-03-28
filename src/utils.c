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
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#include <sys/types.h>
#include <malloc.h>
#define lstat(x,y) _stat(x,y)
#endif

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

char *getNextToken(char *line)
{
    static int i = 0;
    int h = 0;
    char res[2048] = {0};
    ssize_t len;

    if (!line) {
        i = 0;
        return NULL;
    }
    len = strlen(line);
    for (; i < len && (line[i] == ' ' || line[i] == '\t'); ++i);
    for (; i < len && line[i] != ','; ++i) {
        res[h++] = line[i];
    }
    ++i;
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

char *getFileContents(char *file)
{
    ssize_t siz;
    char *res;
    int fd;

    if ((siz = getFileSize(file)) < 0) {
        return NULL;
    }
    res = malloc(sizeof(char) * siz+1);
    if ((fd = open(file, O_RDONLY, 0)) < 0) {
        perror("open");
        fprintf(stderr, "Can't open %s\n", file);
        free(res);
        return NULL;
    }
    if (read(fd, res, sizeof(char) * siz) < 0) {
        perror("read");
        free(res);
        return NULL;
    }
    res[siz] = '\0';
    close(fd);
    return res;
}

char *my_getLine(char *content, size_t *len)
{
    static int i = 0;
    int h = 0;
    char res[1024] = {0}; 

    if (!content) {
        i = 0;
        return NULL;
    }
    for (; content[i] && content[i] != '\n'; ++i) {
        res[h++] = content[i];
    }
    ++i;
    res[h] = '\0';
    *len = h;
    return h ? strdup(res) : NULL;
}
