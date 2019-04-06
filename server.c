#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>

#include "err.h"

#define MAX_CONN 256
#define MAX_BUF 256

int s_fd,
    c_fd;

void handle_exit() 
{
    if(c_fd) 
    {
        close(c_fd);
    }

    if(s_fd)
    {
        close(s_fd);
    }

    printf("\nServer is closing...\n");
    exit(EXIT_SUCCESS);
}

void html_response(char *res, char *msg)
{
    sprintf(res, "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html; charset=utf-8\r\n"
                 "Content-Length: %d\r\n\r\n%s\r\n",
                 (int)strlen(msg), msg);
}

int main(int argc, char **argv)
{
    int port,
        client_len,
        i;

    char msg[MAX_BUF],
         body[MAX_BUF],
         res[MAX_BUF];

    struct sockaddr_in serv_addr,
                       client_addr;

    signal(SIGINT, handle_exit);

    if(argc < 3)
    {
        fprintf(stderr, "More params pls. <PORT> <MSG>\n");
        exit(EXIT_FAILURE);
    }

    port = (int)atol(argv[1]);

    memset(msg, 0, sizeof(msg));
    for(i = 2; argv[i]; i++)
    {
        strcat(msg, argv[i]);

        if(i != argc -1) 
        {
            strcat(msg, " ");
        }
    }

    memset((char*)&serv_addr, 0, sizeof(serv_addr));

    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    check_err(s_fd);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    check_err(bind(s_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)));
    check_err(listen(s_fd, MAX_CONN));

    client_len = sizeof(client_addr);

    for(;;)
    {
        c_fd = accept(s_fd, (struct sockaddr*)&client_addr, (socklen_t*)&client_len);
        check_err(c_fd);

        memset(body, 0, sizeof(body));
        check_err(read(c_fd, body, sizeof(body)));
        
        memset(res, 0, sizeof(res));
        html_response(res, body);
        check_err(write(c_fd, res, sizeof(res)));

        close(c_fd);
    }

    close(s_fd);

    return EXIT_SUCCESS;
}
