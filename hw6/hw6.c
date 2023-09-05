//Jack Byrne

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define MSGSIZE 60

#define bingomsg "BINGO!!!"

void parent(int pfd1[2], int pfd2[2], pid_t child_pid) {

    //set new random seed and pick target num
    srand(time(NULL));
    int target = rand() % 32 + 1;

    int guess;
    char response[MSGSIZE];

    close(pfd1[1]); //close write end of pipe1
    close(pfd2[0]); //close read end of pipe2

    while (1) {
        read(pfd1[0], &guess, sizeof(int));

        if (guess < target) {
            strcpy(response, "too low");
        } else if (guess > target) {
            strcpy(response, "too high");
        } else {
            strcpy(response, bingomsg);
        }

        write(pfd2[1], response, MSGSIZE);

        //break loop upon finding target
        if (guess == target) {
            break;
        }
    }

    close(pfd1[0]); //close read end of pipe1
    close(pfd2[1]); //close write end of pipe2

    int status;
    waitpid(child_pid, &status, 0);
    exit(EXIT_SUCCESS);
}

void child(int pfd1[2], int pfd2[2]) {
    int guess;
    char response[MSGSIZE];

    close(pfd1[0]); //close read end of pipe1
    close(pfd2[1]); //close write end of pipe2

    //loop to check scan responses and check responses
    while (1) {
        printf("Enter your guess (between 1-32): ");
        scanf("%d", &guess);

        write(pfd1[1], &guess, sizeof(int));
        read(pfd2[0], response, MSGSIZE);

        printf("Response: %s\n", response);

        if (strcmp(response, bingomsg) == 0) {
            break;
        }
    }

    close(pfd1[1]); //close write end of pipe1
    close(pfd2[0]); //close read end of pipe2

    exit(EXIT_SUCCESS);
}

int main() {
    int pfd1[2], pfd2[2];
    pid_t pid;

    if (pipe(pfd1) < 0 || pipe(pfd2) < 0) {
        perror("error creating the pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0) {
        perror("error forking");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        child(pfd1, pfd2);
    } else {
        parent(pfd1, pfd2, pid);
    }

    return EXIT_SUCCESS;
}
