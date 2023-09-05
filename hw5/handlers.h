#include <unistd.h>
#include <signal.h>
#include <sys/signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <pwd.h>
#include <errno.h>

void handle_sigint(int sig) {
    printf("\n The minishell cannot be terminated using Ctrl+C\n");
}

void handle_sigchld(int sig) {
    int saved_errno = errno;
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("pid %d done\n", pid);
    }

    errno = saved_errno;
}

void handle_cd(int argc, char *argv[]) {
    if (argc > 2) {
        fprintf(stderr, "Error: Too many arguments to cd.\n");

    } else if (argc == 1 ||strcmp(argv[1], "~") == 0) {
        struct passwd *pw = getpwuid(getuid());
        if (chdir(pw->pw_dir) == -1) {
            fprintf(stderr, "Error: Cannot change directory to %s. %s.\n", pw->pw_dir, strerror(errno));
        }
    } else {
        if (chdir(argv[1]) == -1) {
            fprintf(stderr, "Error: Cannot change directory to %s. %s.\n", argv[1], strerror(errno));
        }
    }
}

void handle_exit(int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "&") == 0) {
            fprintf(stderr, "Error: exit cannot be followed by &.\n");
        } else {
            fprintf(stderr, "Error: exit takes no arguments.\n");
        }
    } else {
        exit(0);
    }
}