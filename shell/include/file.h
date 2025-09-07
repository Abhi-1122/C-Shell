#ifndef FILE_H
#define FILE_H

#include <fcntl.h>
#include <unistd.h>

int open_file_for_read(const char *filename);
int open_file_for_write(const char *filename);
int open_file_for_append(const char *filename);
void close_file(int fd);
int redirect_stdin(int fd);
int redirect_stdout(int fd);
int restore_stdin(int saved_fd);
int restore_stdout(int saved_fd);

#endif
