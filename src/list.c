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

#include "list.h"

static void alonexec_specFree(alonexec_spec *spec)
{
    free(spec->idname);
    free(spec->src);
    free(spec->dst);
    free(spec->perms);
    free(spec->content);
    free(spec);
}

alonexec_list_t *alonexec_listInsert(alonexec_list_t **l, alonexec_spec *spec)
{
    alonexec_list_t *tmp, *last = NULL;

    for (tmp = *l; tmp; tmp = tmp->next)
        last = tmp;
    tmp = malloc(sizeof(alonexec_list_t));
    tmp->next = NULL;
    tmp->data = spec;
    if (last)
        last->next = tmp;
    return tmp;
}

void alonexec_listFree(alonexec_list_t *l)
{
    alonexec_list_t *tmp = l, *tmp2;

    while (tmp) {
        alonexec_specFree(tmp->data), tmp->data = NULL;
        tmp2 = tmp;
        tmp = tmp->next;
        free(tmp2), tmp2 = NULL;
    }
    free(tmp), tmp = NULL;
}
