#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>

#define MAX_CONN 256
#define MAX_BUF 256

void check_err(int i) 
{
    if (i < 0)
    {
        perror("Error: \n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    int port,
        s_fd,
        c_fd,
        client_len,
        i;

    char msg[MAX_BUF],
         res[MAX_BUF];

    struct sockaddr_in serv_addr,
                      client_addr;

    if(argc < 3)
    {
        fprintf(stderr, "More params pls. <PORT> <MSG>\n");
        exit(EXIT_FAILURE);
    }

    port = (int)atol(argv[1]);

    for(i = 2; argv[i]; i++)
    {
        strcat(msg, argv[i]);

        if(i != argc -1) 
        {
            strcat(msg, " ");
        }
    }

    void handle() 
    {
        close(c_fd);
        close(s_fd);
        printf("\nServer is closing...\n");
        exit(EXIT_SUCCESS);
    }

    signal(SIGINT, handle);
    
    memset((char*)&serv_addr, 0, sizeof(serv_addr));

    sprintf(res, "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html; charset=utf-8\r\n"
                 "Content-Length: %d\r\n\r\n%s\r\n",
                 (int)strlen(msg), msg);

    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    check_err(s_fd);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    check_err(bind(s_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)));
    check_err(listen(s_fd, MAX_CONN));

    client_len = sizeof(client_addr);

    while(1) 
    {
        c_fd = accept(s_fd, (struct sockaddr*)&client_addr, (socklen_t*)&client_len);
        check_err(c_fd);

        check_err(write(c_fd, res, strlen(res)));
        close(c_fd);
    }

    close(s_fd);

    return EXIT_SUCCESS;
}
