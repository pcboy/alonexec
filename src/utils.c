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

#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

char *removeQuotes(char *line)
{
    char *res;
    int i, h = 0;

    if (!line)
        return NULL;
    res = alloca(strlen(line)+1);
    for (i = 0; line[i]; ++i) {
        if (line[i] != '"') {
            res[h++] = line[i];
        }
    }
    res[h] = '\0';
    return h ? strdup(res) : NULL;
}

char *removeBadChars(char *line)
{
    char *res; 
    int i, h = 0;

    if (!line)
        return NULL;
    res = alloca(strlen(line)+1);
    for (i = 0; line[i]; ++i) {
        if (isalpha(line[i])) { /* FIXME: Will causes problem in the future.
                                  What to do for 8c,9c binary names for instance ?*/
            res[h++] = line[i];
        }
    }
    res[h] = '\0';
    return h ? strdup(res) : NULL;
}

char *getNextToken(char *line)
{
    static int i = 0;
    int h = 0;
    char res[2048] = {0};

    if (!line) {
        i = 0;
        return NULL;
    }
    for (; line[i] && (line[i] == ' ' || line[i] == '\t'); ++i);
    for (; line[i] && line[i] != ','; ++i) {
        res[h++] = line[i];
    }
    ++i;
    res[h] = '\0';
    return h ? strdup(res) : NULL;
}

size_t getFileSize(char *file)
{
    struct stat s;

    if (lstat(file, &s) < 0) {
        perror("lstat");
        fprintf(stderr, "Can't lstat %s\n", file);
        return 0;
    }
    return s.st_size;
}

char *getFileContents(char *file)
{
    size_t siz = getFileSize(file);
    char *res = malloc(sizeof(char) * siz+1);
    int fd;

    if ((fd = open(file, O_RDONLY, 0)) < 0) {
        perror("open");
        fprintf(stderr, "Can't open %s\n", file);
        return NULL;
    }
    read(fd, res, sizeof(char) * siz);
    res[siz] = '\0';
    close(fd);
    return res;
}

char *my_getLine(char *content, size_t *len)
{
    static int i = 0;
    int h = 0;
    char res[1024] = {0}; 

    if (!content) {
        i = 0;
        return NULL;
    }
    for (; content[i] && content[i] != '\n'; ++i) {
        res[h++] = content[i];
    }
    ++i;
    res[h] = '\0';
    *len = h;
    return h ? strdup(res) : NULL;
}
