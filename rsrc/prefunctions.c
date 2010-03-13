/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * See the COPYING file for license information.
 *
 * David Hagege <pcboy.pebkac@gmail.com>
 */

#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>

#if !defined(_WIN32)
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
#endif

mode_t str2mode(char *rights)
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

int executeRsrc(const char *file, char * argv[])
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

void copyRsrc(const char *src, const char *dst, char *perms,
        char *content, size_t len)
{
    FILE *fp = fopen(dst, "w");
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
