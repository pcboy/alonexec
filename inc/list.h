#ifndef __LIST_H__
#define __LIST_H__

#include "../rsrc/spec.h"

typedef struct alonexec_list_s {
    void *data;
    struct alonexec_list_s *next;
} alonexec_list_t;

alonexec_list_t *alonexec_listInsert(alonexec_list_t **l, alonexec_spec *spec);
void alonexec_listFree(alonexec_list_t *l);

#endif /* __LIST_H__ */

