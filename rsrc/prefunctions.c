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
#include <stdio.h>

int executeRsrc(const char *file)
{
    pid_t pid = fork();

    printf("executing %s\n", file);
    switch (pid) {
        case 0:
            execl(file, file, NULL);
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
    printf("Copying %s to %s with %s\n", src, dst, perms);
    fwrite(content, sizeof(char), len, fp);
    fclose(fp);
}
