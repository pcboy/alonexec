#ifndef _ALONEXEC_H_
#define _ALONEXEC_H_

#include <stdio.h>
#include <sys/param.h>

#include "list.h"

#define ALONEXEC_WORKDIR "/tmp/"
#define ALONEXEC_SPECFILE "../rsrc/spec.h"
#define ALONEXEC_FCTFILE "../rsrc/prefunctions.c"

typedef struct alonexec_s {
    int (*compile)(struct alonexec_s*);
    /**/
    char *tpl;
    char genfile[MAXPATHLEN];
    FILE *fgenfile;
    alonexec_list_t *listfiles;
} alonexec_t;

alonexec_t *alonexec_init(char *tpl, char **opts);
void alonexec_destroy(alonexec_t*);

#endif /* _ALONEXEC_H_ */
