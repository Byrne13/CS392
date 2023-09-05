//Jack Byrne
//client.c 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {

    int sock;
    struct sockaddr_in server;
    float num, sum;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);

    //create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        fprintf(stderr, "error creating socket %s", strerror(errno));
        return EXIT_FAILURE;
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    //connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        fprintf(stderr, "connect failed %s", strerror(errno));
        return EXIT_FAILURE;
    }
    puts("Connected");

    //infinite loop to communicate with server
    while (1) {
        printf("Enter a number to add: ");
        scanf("%f", &num);

        //send data
        if (send(sock, &num, sizeof(float), 0) < 0) {
            fprintf(stderr, "send failed %s", strerror(errno));
            return EXIT_FAILURE;
        }

        //receive a reply
        if (recv(sock, &sum, sizeof(float), 0) < 0) {
            fprintf(stderr, "receive failed %s", strerror(errno));
            return EXIT_FAILURE;
        }

        printf("Current sum: %f\n", sum);
    }

    //close
    if (close(sock) < 0) {
        fprintf(stderr, "close failed %s", strerror(errno));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


