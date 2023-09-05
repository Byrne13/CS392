#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>


int main(int argc, char *argv[]) {

    /*
      TODO:
      Use fork() and pipe(), but NOT popen().

      Create a pipe through which a child process will send two integers to 
      the parent. The parent will add the integers and print the result to 
      the terminal. 

      OPTIONAL:
      Make a second child, which will do the addition. In this case, the parent
      waits for the children to terminate. (See Lab 6.)
    */
    pid_t pid, pid2;
    int fd[2];

    if(pipe(fd) < 0){
      perror("pipe failure");
      return EXIT_FAILURE;
    }

    pid = fork();

    if (pid < 0) { //error
      perror("fork failure");
      return EXIT_FAILURE;
    } else if (pid == 0){ //child process
      int num1 = 13;
      int num2 = 31;

      //close read end
      close(fd[0]);

      //write ints to pipe
      write(fd[1], &num1, sizeof(int));
      write(fd[1], &num2, sizeof(int));

      //close write end
      close(fd[1]);

      return EXIT_SUCCESS;
    } else { //parent

      //make second child
      pid = fork();

      if (pid2 < 0) { //fork 2 failure
            perror("fork failure");
            return EXIT_FAILURE;
        } else if (pid2 == 0) { //Second child
              int num1, num2, sum;

              //close write end
              close(fd[1]);

              //write ints to pipe
              read(fd[0], &num1, sizeof(int));
              read(fd[0], &num2, sizeof(int));

              //close read
              close(fd[0]);

              sum = num1 + num2;

              printf("sum: %d\n", sum);

              return EXIT_SUCCESS;
          } else {//Parent2
            int status;

            //wait for children to terminate
            waitpid(pid, &status, 0);
            waitpid(pid2, &status, 0);
        }
      }
      return EXIT_SUCCESS;
}

    
  
