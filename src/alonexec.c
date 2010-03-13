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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#ifndef _WIN32
#include <sys/wait.h>
#endif
#ifdef _WIN32
#include <windows.h>
#define mkstemp(x) _mktemp(x)
#endif

#include "alonexec.h"
#include "utils.h"
#include "list.h"
#include "../rsrc/spec.h"

static void alonexec_writeMain(alonexec_t *slf)
{
    /* FIXME: For now we are chdiring() for facilities. */
    fprintf(slf->fgenfile, "int main(int argc, char *argv[]) {\n\
            int i;\n\
            const char *tmp = getTempDirectory();\n\
            if (chdir(tmp) < 0){\n\
                perror(\"chdir\");\n\
            }\n\
            for (i = 0; alonefiles[i].src; ++i) {\n\
                copyRsrc(alonefiles[i].src, alonefiles[i].dst,\n\
                    alonefiles[i].perms, alonefiles[i].content,\n\
                    alonefiles[i].contentlen);\n\
            }\n\
            for (i = 0; alonefiles[i].src; ++i) {\n\
                if (alonefiles[i].exec)\n\
                    executeRsrc(alonefiles[i].dst, argv);\n\
            }\
            return 0;\
            }");
}

static void alonexec_writeFunctions(alonexec_t *slf)
{
    char *fcts = getFileContents(ALONEXEC_FCTFILE);
    
    fprintf(slf->fgenfile, "%s", fcts);
    free(fcts);
    alonexec_writeMain(slf);
}

static void alonexec_writeSpecTable(alonexec_t *slf)
{
    char *speccontent = getFileContents(ALONEXEC_SPECFILE);
    alonexec_list_t *it;

    fprintf(slf->fgenfile, "%s", speccontent);
    fprintf(slf->fgenfile, "alonexec_spec alonefiles[] = {\n");
    for (it = slf->listfiles; it; it = it->next) {
        alonexec_spec *spec = it->data;
        fprintf(slf->fgenfile, "{%s,%s,%s,%i,%s,%i},\n", spec->src, spec->dst,
                spec->perms, spec->exec, spec->content, spec->contentlen);
    }
    fprintf(slf->fgenfile, "{NULL, NULL, NULL, 0, NULL, 0}");
    fprintf(slf->fgenfile, "\n};\n");
    free(speccontent);
}

static void alonexec_writeRsrc(alonexec_t *slf, alonexec_spec *spec)
{
    char *stripname, *filename;
    char *rsrc;
    char *content;
    int i;
    ssize_t siz;
    ssize_t wrotelen = 0;

    stripname = removeChars(spec->src, isalpha);
    filename = removeChars(spec->src, notQuote);
    content = getFileContents(filename);
    printf("Packing %s\n", stripname);
    fprintf(slf->fgenfile, "static char %s[] = {", stripname);
    if ((siz = getFileSize(filename)) < 0) {
        fprintf(stderr, "%s:%i Can't get %s file size.\n",
                __FILE__, __LINE__, filename);
        free(stripname);
        free(filename);
        free(content);
        return;
    }
    spec->content = stripname ? strdup(stripname) : NULL;
    spec->contentlen = siz;
    rsrc = malloc(sizeof(char) * spec->contentlen*6);
    for (i = 0; i < siz; ++i) {
        char oct[8] = {0};
        ssize_t wrote;
        wrote = snprintf(oct, sizeof(oct), "0x%x%c",
                content[i] & 0xff, (i+1 != siz ? ',' : ' '));
        memcpy(rsrc + wrotelen, oct, wrote);
        wrotelen += wrote;
    }
    fwrite(rsrc, sizeof(char), wrotelen, slf->fgenfile);
    fprintf(slf->fgenfile, "};\n");
    free(stripname);
    free(filename);
    free(content);
    free(rsrc);
}

static void alonexec_writeAllRsrc(alonexec_t *slf)
{
    alonexec_list_t *it;

    for (it = slf->listfiles; it; it = it->next) {
        alonexec_writeRsrc(slf, it->data);
    }
}

static void alonexec_parseTpl(alonexec_t* slf, char *tpl)
{
    char *tplcontent = getFileContents(tpl);
    size_t len = 0;
    int pass = 0;
    char *line;
    bool first = true;

    my_getLine(NULL, &len);

    while ((line = my_getLine(tplcontent, &len))) {
        if (line[0] == '#') /* It's a comment. */
            continue;
        char *token;
        char *p = line;
        alonexec_spec *spec = malloc(sizeof(alonexec_spec));
        while (*p && *p != '"') p++;
        while ((token = getNextToken(p))) {
            ++pass;
            switch(pass) {
                case 1:
                    spec->src = strdup(token);
                    break;
                case 2:
                    spec->dst = strdup(token);
                    break;
                case 3:
                    spec->perms = strdup(token);
                    break;
                case 4:
                    spec->exec = (token[0] == 't' ? true : false);
                    break;
            }
            free(token);
        }
        pass = 0;
        if (first) {
            slf->listfiles = alonexec_listInsert(&(slf->listfiles), spec);
            first = false;
        } else {
            alonexec_listInsert(&(slf->listfiles), spec);
        }
        getNextToken(NULL);
        free(line);
    }
    free(tplcontent);
}

#if defined(_WIN32)
static int alonexec_compile(alonexec_t *slf)
{
    STARTUPINFO info;
    PROCESS_INFORMATION processInfo;
    char cmd[2048] = {0};

    snprintf(cmd, sizeof(cmd), "tcc %s -o finalexe.exe", slf->genfile);
    if (!CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL,
                &info, &processInfo))
    {
        fprintf(stderr, "Can't CreateProcess(): %i\n", GetLastError());
        return -1;
    }
    WaitForSingleObject(processInfo.hProcess, INFINITE);
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
    return 0;
}
#else
static int alonexec_compile(alonexec_t *slf)
{
    int status = 0;
    pid_t pid = fork();
   
    switch (pid) {
        case 0:
            printf("Compiling final executable...\n");
            execl("tcc", "tcc", "-W", "-Wall", slf->genfile,
                    "-o", "finalexe", NULL);
            return -1;
        case -1:
            perror("fork");
            return -1;
        default:
            wait(&status);
            return 0;
    }
}
#endif

alonexec_t *alonexec_init(char *tpl, char **opts)
{
    alonexec_t *res;

    (void)opts;
    res = malloc(sizeof(alonexec_t));
    res->tpl = tpl;
    res->listfiles = NULL;
    snprintf(res->genfile, sizeof(res->genfile),
            "%s/%s", getTempDirectory(), "alonexecgen.c");
    if (!(res->fgenfile = fopen(res->genfile, "w+"))) {
        perror("fopen");
        fprintf(stderr, "%i:%s Can't open %s\n", __LINE__,
                __FUNCTION__, res->genfile);
        exit(EXIT_FAILURE);
    }
    alonexec_parseTpl(res, tpl);
    alonexec_writeAllRsrc(res);
    alonexec_writeSpecTable(res); /*XXX:SpecTable must be written after Rsrc.*/
    alonexec_writeFunctions(res);
    res->compile = alonexec_compile;
    fclose(res->fgenfile);
    return res;
}

void alonexec_destroy(alonexec_t *del)
{
#if 0
XXX: Remove temporary directory.

    if (unlink(del->genfile) < 0) {
        fprintf(stderr, "%s:%i Can't unlink %s\n", __FUNCTION__,
                __LINE__, del->genfile);
    }
#endif
    alonexec_listFree(del->listfiles);
    free(del), del = NULL;
}

