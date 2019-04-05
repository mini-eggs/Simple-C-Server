#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h> // read/write
#include <signal.h>

#define BUF_MAX 256

int main (int argc, char** argv) {
    if (argc < 3) {
        printf("Not enough arguments. Try <PORT> <MSG>.\n");
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);

    char msg[BUF_MAX];
    char body[BUF_MAX];
    char res[BUF_MAX];

    // construct echo message
    strcpy(msg, argv[2]);
    for(int i = 3; i < argc; i++) {
        strcat(msg, " ");
        strcat(msg, argv[i]);
    }

    sprintf(res, "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html; charset=utf-8\r\n"
                 "Content-Length: %d\r\n\r\n%s\r\n", 
                 (int)strlen(msg), msg);

    int server_fd, client_fd, client_length, status;
    struct sockaddr_in serv_addr, client_addr;

    void handle_exit(){
        close(client_fd);
        close(server_fd);
        printf("\n");
        exit(EXIT_SUCCESS);
    }
    signal(SIGINT, handle_exit);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0) {
        printf("Error opening socket.\n");
        return EXIT_FAILURE;
    }

    bzero((char*)&serv_addr,sizeof(serv_addr)); // zero out

    serv_addr.sin_family= AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if(bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error could not bind.\n");
        return EXIT_FAILURE;
    }

    listen(server_fd, 5);
    client_length = sizeof(client_addr);

    while(1) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_length);
        if(client_fd < 0) {
            printf("Error could not accept.\n");
            return EXIT_FAILURE;
        }

        // received
        bzero(body, BUF_MAX); // zero out
        status = read(client_fd, body, BUF_MAX);
        if(status < 0) {
            printf("Error reading from socket.\n");
            return EXIT_FAILURE;
        }

        // sending
        status = write(client_fd, res, strlen(res)); 
        if(status < 0) {
            printf("Error writing to socket.\n");
            return EXIT_FAILURE;
        }


        close(client_fd);
    }

    return EXIT_SUCCESS;
}
