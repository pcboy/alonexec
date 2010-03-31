#ifndef __UTILS_H__
#define __UTILS_H__

int notQuote(int c);
const char *getTempDirectory(void);
char *removeChars(char *line, int (*keepEq)(int));
char *getNextToken(char *line);
ssize_t getFileSize(char *file);
char *getFileContents(char *file);
char *my_getLine(char *content, size_t *len);
unsigned long hash(unsigned char *str);
char *lowercase(char *str);

#endif /* __UTILS_H__ */

