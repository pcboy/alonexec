#ifndef _ALONEXEC_SPEC_
#define _ALONEXEC_SPEC_

#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct alonexec_spec {
    char *idname;
    char *src;
    char *dst;
    char *perms;
    bool exec;
    char *content;
    size_t contentlen;
} alonexec_spec;

#endif /* _ALONEXEC_SPEC_ */
