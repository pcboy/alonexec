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
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#include <malloc.h>
#define mkdir(x,y) mkdir(x)

#define CROSS_SLASH '\\'
#else
#define CROSS_SLASH '/'
#endif

#ifdef __linux__
#include <alloca.h>
#include <linux/limits.h>
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

const char *getTempDirectory(int id)
{
    static char *tmp = NULL;
    static char tmpf[2048] = {0};

    if (tmp)
        return tmp;
    if (!(tmp = getenv("TMPDIR"))) {
#if defined(_WIN32)
        tmp = alloca(2048);
        GetTempPath(2048, tmp);
#else
        tmp = P_tmpdir;
#endif
    }
    snprintf(tmpf, sizeof(tmpf), "%s%calonexectmp.%i",
            tmp, CROSS_SLASH, id);
    mkdir(tmpf, 0755); /* XXX: Check if already exists */
    return tmpf;
}

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

int crossExec(const char *file, char *const argv[])
{
    int pid;

#if defined(__linux__)
    extern char **environ;

    pid = fork();
    switch (pid) {
        case 0:
            execve(file, argv, environ);
            fprintf(stderr, "Can't exec %s\n", file);
            perror("execve");
            exit(EXIT_FAILURE);
            break;
        case -1:
            fprintf(stderr, "Can't fork()\n");
            perror("fork");
            break;
    }
    return pid;
#elif defined(_WIN32)
    STARTUPINFO info;
    PROCESS_INFORMATION procinfo;
    char cmd[MAX_PATH] = {0};
    size_t i;

    memset(&info, 0, sizeof(info));
    memset(&procinfo, 0, sizeof(procinfo));
    snprintf(cmd, sizeof(cmd), "%s ");
    for (i = 0; argv[i]; ++i) {
        strncat(cmd, argv[i], sizeof(cmd));
        strncat(cmd, " ", sizeof(cmd));
    }
    if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &info,
                &(procinfo)))
    {
        fprintf(stderr, "Can't exec %s\n", file);
    }
    return (int)procinfo.hProcess;
#endif
}


static int executeRsrc(const char *file, char * argv[])
{
#ifndef NDEBUG
    printf("executing %s\n", file);
#endif
    return crossExec(file, argv);
}

static void recurseMkdir(const char *dir)
{
    char tmp[PATH_MAX] = {0};
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
  
    (void)src;
    strncpy(folder, dst, sizeof(folder));
    for (i = 0; folder[i]; ++i);
    for (--i; i && folder[i] != CROSS_SLASH; --i);
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
#ifndef NDEBUG
    printf("Copying %s to %s with %s\n", src, dst, perms);
#endif
    fwrite(content, sizeof(char), len, fp);
    fclose(fp);
    if (chmod(dst, str2mode(perms)) < 0) {
        perror("chmod");
        fprintf(stderr, "Can't chmod %s to %s\n", dst, perms);
    }
}

