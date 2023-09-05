//Jack Byrne

#include <unistd.h>
#include <signal.h>
#include <sys/signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <pwd.h>
#include <errno.h>

#include "handlers.h"


void print_prompt() {
    char cwd[1024];
    if (!getcwd(cwd, sizeof(cwd))) {
        fprintf(stderr, "Error: Cannot get current working directory. %s.\n", strerror(errno));
        exit(1);
    }

    //get username
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    if (pw == NULL) {
        fprintf(stderr, "Error: Cannot get passwd entry. %s.\n", strerror(errno));
        exit(1);
    }

    printf("minishell:%s:%s$ ", pw->pw_name, cwd);
}


int main() {

    //register signal handlers
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        fprintf(stderr, "Error: Cannot register signal handler. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    if (signal(SIGCHLD, handle_sigchld) == SIG_ERR) {
        fprintf(stderr, "Error: Cannot register signal handler. %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    //infinite loop to prompt user
    while (1) {
        print_prompt();

        char input[1024];
        if (!fgets(input, sizeof(input), stdin)) {
            fprintf(stderr, "Error: Failed to read from stdin. %s.\n", strerror(errno));
            return EXIT_FAILURE;
        }

        //remove newline character
        char *newline = strchr(input, '\n');
        if (newline) {
            *newline = 0;
        }

        //background checker
        int isBackground = 0;
        char *amp = strchr(input, '&');
        if (amp && *(amp + 1) == 0) {
            *amp = 0;
            isBackground = 1;
        }

        //tokenize input string
        char *token = strtok(input, " ");
        char *argv[64] = {0};
        int argc = 0;
        while (token) {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }

        //no entry
        if (argc == 0){ 
            continue;
        }

        //recognize built-in commands cd and exit
        if (strcmp(argv[0], "cd") == 0) {
            handle_cd(argc, argv);
        } else if (strcmp(argv[0], "exit") == 0) {
            handle_exit(argc, argv);
            break;
        } else {
            pid_t pid = fork();
            if (pid < 0) { //Error
                    fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
            } else if (pid == 0) { //child
                if (execvp(argv[0], argv) == -1) {
                    fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
                    return EXIT_FAILURE;
                }
            } else { //parent
                if (!isBackground) {
                    if (waitpid(pid, NULL, 0) == -1) {
                        fprintf(stderr, "Error: wait() failed. %s.\n", strerror(errno));
                    }
                } else {
                    printf("pid: %d cmd: %s\n", pid, argv[0]);
                }
            } 
        }
    }

    killpg(getpid(), SIGTERM);
    return 0;
}