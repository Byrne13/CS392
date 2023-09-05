//Jack Byrne
//server.c 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    
    int sd, csd, c;
    struct sockaddr_in server, client;
    float number_received, sum = 0;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    //get port num
    int port = atoi(argv[1]);

    //create socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1) {
        fprintf(stderr, "error creating socket %s", strerror(errno));
        return EXIT_FAILURE;
    }
    puts("Socket created");

    //prepare sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    //bind
    if (bind(sd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        fprintf(stderr, "bind failed %s", strerror(errno));
        return EXIT_FAILURE;
    }
    puts("Bind done");

    //listen
    listen(sd, 3);

    //accept an incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    //accept connection from client
    csd = accept(sd, (struct sockaddr *)&client, (socklen_t*)&c);
    if (csd < 0) {
        fprintf(stderr, "accept failed %s", strerror(errno));
        return EXIT_FAILURE;
    }
    puts("Connection accepted");

    //loop receiving message
    while (recv(csd, &number_received, sizeof(float), 0) > 0) {
        sum += number_received;
        //send sum to client
        if (send(csd, &sum, sizeof(float), 0) < 0) {
            fprintf(stderr, "send failed %s", strerror(errno));
            return EXIT_FAILURE;
        }
    }

    //close
    if (close(csd) < 0) {
        fprintf(stderr, "close failed %s", strerror(errno));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
