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

#include "alonexec.h"

static void usage(char *argv[])
{
    fprintf(stderr, "usage: %s <file.alonexec>\n"
            "An alonexe file use an s-expression syntax like that:\n"
            "(name (sourcepath \"./namefile\") (destpath \"./destpath\")\n"
            "(mode \"rwxr-xr-x\") (autostart true))\n"
            "\nhttp://bitbucket.org/pcboy/alonexec for more information.\n"
            , argv[0]);
}

int main(int argc, char *argv[])
{
    if (argc > 1) {
        alonexec_t *alone = alonexec_init(argv[1], argv);
        alone->compile(alone);
        alonexec_destroy(alone);
        return EXIT_SUCCESS;
    }
    usage(argv);
    return EXIT_FAILURE;
}
