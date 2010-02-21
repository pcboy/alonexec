#ifndef __UTILS_H__
#define __UTILS_H__

int notQuote(int c);
char *removeChars(char *line, int (*keepEq)(int));
char *getNextToken(char *line);
ssize_t getFileSize(char *file);
char *getFileContents(char *file);
char *my_getLine(char *content, size_t *len);

#endif /* __UTILS_H__ */

