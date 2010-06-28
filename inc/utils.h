#ifndef __UTILS_H__
#define __UTILS_H__

typedef struct file_s {
    int fd;
    char *data;
    size_t len;
} file_t;

int notQuote(int c);
const char *getTempDirectory(int id);
char *removeChars(char *line, int (*keepEq)(int));
char *getNextToken(char *line);
ssize_t getFileSize(char *file);
file_t *getFileContents(char *file);
int closeFile(file_t *f);
char *my_getLine(char *content, size_t *len);
unsigned long hash(unsigned char *str);
char *lowercase(char *str);

#endif /* __UTILS_H__ */

