#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h> // read/write
#include <signal.h>

#define BUF_MAX 256

int true_sizeof(char *str) {
    int pos= 0;
    char curr;
    while((curr = str[pos]) && curr != '\0') {
        pos++;
    }
    return pos;
}

int main (int argc, char** argv) {
    if (argc < 3) {
        printf("Not enough arguments. Try <PORT> <MSG>.\n");
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    char msg[BUF_MAX];

    strcpy(msg, argv[2]);

    for(int i = 3; i < argc; i++) {
        strcat(msg, " ");
        strcat(msg, argv[i]);
    }

    int sockfd, newsockfd, clilen, n;
    char buf[BUF_MAX];
    struct sockaddr_in serv_addr, cli_addr;

    char res[BUF_MAX];
    sprintf(res, "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: %d\r\n\r\n%s\r\n", true_sizeof(msg), msg);

    void handle_exit(){
        close(newsockfd);
        close(sockfd);
        printf("\n");
        exit(EXIT_SUCCESS);
    }

    signal(SIGINT, handle_exit);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0) {
        printf("Error opening socket.\n");
        return EXIT_FAILURE;
    }

    bzero((char*)&serv_addr,sizeof(serv_addr)); // zero out

    serv_addr.sin_family= AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error could not bind.\n");
        return EXIT_FAILURE;
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while(1) {
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
        if(newsockfd < 0) {
            printf("Error could not accept.\n");
            return EXIT_FAILURE;
        }

        // received
        bzero(buf, BUF_MAX); // zero out
        n = read(newsockfd, buf, BUF_MAX);
        if(n < 0) {
            printf("Error reading from socket.\n");
            return EXIT_FAILURE;
        }

        // sending
        n = write(newsockfd, res, true_sizeof(res)); 
        if(n < 0) {
            printf("Error writing to socket.\n");
            return EXIT_FAILURE;
        }


        close(newsockfd);
    }

    return EXIT_SUCCESS;
}
