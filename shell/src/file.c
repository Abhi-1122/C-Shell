#include "log.h"
#include "parser.h"
#include "hop.h"
#include "reveal.h"
#include "file.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int open_file_for_read(const char *filename) {
    return open(filename, O_RDONLY);
}

int open_file_for_write(const char *filename) {
    return open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
}

int open_file_for_append(const char *filename) {
    return open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
}

void close_file(int fd) {
    if (fd >= 0) {
        close(fd);
    }
}


int redirect_stdin(int fd) {
    int saved_stdin = dup(STDIN_FILENO);
    if (saved_stdin == -1) {
        perror("dup stdin");
        return -1;
    }
    if (dup2(fd, STDIN_FILENO) == -1) {
        perror("dup2 stdin");
        close(saved_stdin);
        return -1;
    }
    return saved_stdin;
}

int redirect_stdout(int fd) {
    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1) {
        perror("dup stdout");
        return -1;
    }
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2 stdout");
        close(saved_stdout);
        return -1;
    }
    return saved_stdout;
}

int restore_stdin(int saved_fd) {
    if (dup2(saved_fd, STDIN_FILENO) == -1) {
        perror("restore stdin");
        return -1;
    }
    close(saved_fd);
    clearerr(stdin);
    return 0;
}

int restore_stdout(int saved_fd) {
    if (dup2(saved_fd, STDOUT_FILENO) == -1) {
        perror("restore stdout");
        return -1;
    }
    close(saved_fd);
    clearerr(stdout);
    return 0;
}