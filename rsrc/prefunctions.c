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

#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <alloca.h>
#include <string.h>

#if defined(_WIN32)
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif

#ifdef __linux__
#include <linux/limits.h>
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#if !defined(_WIN32)
static const char *getTempDirectory(void)
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
#endif

static mode_t str2mode(char *rights)
{
    static mode_t res = 0;
    mode_t tmp = 0;
    static int pos = 6;
    int i = 0;

    while (rights[i]) {
        if (i && !(i%3)) {
            break;
        }
        switch(rights[i]) {
            case 'r':
                tmp |= 4;
                break;
            case 'w':
                tmp |= 2;
                break;
            case 'x':
                tmp |= 1;
                break;
            default:
                break;
        }
        ++i;
    }
    res |= ((tmp << pos));
    pos -= 3;
    if (i && pos >= 0)
        return str2mode(rights + i);
    pos = 6;
    tmp = res;
    res = 0;
    return tmp;
}

static int executeRsrc(const char *file, char * argv[])
{
    pid_t pid = fork();

    switch (pid) {
        case 0:
            printf("executing %s\n", file);
            argv[0] = (char*)file;
            execv(file, argv);
            return -1;
        case -1:
            perror("fork");
            return -1;
        default:
            return 0;
    }
}

static void recurseMkdir(const char *dir)
{
    char tmp[PATH_MAX];
    char *p = NULL;
    size_t len;

    strncpy(tmp, dir, sizeof(tmp));
    len = strlen(tmp);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    for (p = tmp + 1; *p; p++)
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
        }
    mkdir(tmp, S_IRWXU);
}

static void copyRsrc(const char *src, const char *dst, char *perms,
        char *content, size_t len)
{
    FILE *fp;
    int i;
    char folder[PATH_MAX];
   
    strncpy(folder, dst, sizeof(folder));
    for (i = 0; folder[i]; ++i);
    for (--i; i && folder[i] != PATH_SEPARATOR; --i);
    folder[i] = '\0';
    if (i) {
        recurseMkdir(folder);
    }
    fp = fopen(dst, "w");
    if (!fp) {
        perror("fopen");
        fprintf(stderr, "Can't fopen %s\n", dst);
        return;
    }
    printf("Copying %s to %s with %s\n", src, dst, perms);
    fwrite(content, sizeof(char), len, fp);
    fclose(fp);
    if (chmod(dst, str2mode(perms)) < 0) {
        perror("chmod");
        fprintf(stderr, "Can't chmod %s to %s\n", dst, perms);
    }
}

static const char *lastCurrentDir(bool reset)
{
    static const char dir[PATH_MAX] = {0};
    static char *res = NULL;

    if (res && !reset)
        return res;
    res = getcwd(res, sizeof(res));
    return res;
}

