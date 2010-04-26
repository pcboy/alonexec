#ifndef _ALONEXEC_H_
#define _ALONEXEC_H_

#include <stdio.h>
#include <sys/param.h>

#include "list.h"

#define ALONEXEC_SPECFILE "../rsrc/spec.h"
#define ALONEXEC_FCTFILE "../rsrc/prefunctions.c"

#define CONFIG_SOURCEPATH 3090735331lu
#define CONFIG_DESTPATH 953670466lu
#define CONFIG_MODE 2090515018lu
#define CONFIG_AUTOSTART 4228650444lu
#define CONFIG_ALONEXEC 2675134154lu

typedef struct alonexec_s {
    int (*compile)(struct alonexec_s*);
    /**/
    char *tpl;
    char genfile[MAXPATHLEN];
    FILE *fgenfile;
    char *destbinary;
    alonexec_list_t *listfiles;
} alonexec_t;

alonexec_t *alonexec_init(char *tpl, char **opts);
void alonexec_destroy(alonexec_t*);

#endif /* _ALONEXEC_H_ */
