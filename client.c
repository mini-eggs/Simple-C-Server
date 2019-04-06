#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#include "err.h"

int main(int argc, char **argv)
{
    int s_fd,
        status,
        port;

    char msg[256];

    struct sockaddr_in serv_addr;

    if(argc < 2)
    {
        fprintf(stderr, "Not enough arguments. Please provide port. <PORT>\n");
        return EXIT_FAILURE;
    }

    port = (int)atol(argv[1]);

    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; 
    serv_addr.sin_port = htons(port);

    for(;;)
    {
        s_fd = socket(AF_INET, SOCK_STREAM, 0);
        check_err(s_fd);

        status = connect(s_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        check_err(status);

        memset(msg, 0, sizeof(msg));
        check_err(read(STDIN_FILENO, msg, sizeof(msg)));
        write(s_fd, msg, sizeof(msg));

        memset(msg, 0, sizeof(msg));
        check_err(read(s_fd, msg, sizeof(msg)));

        printf("\nRECEIVED:\n%s\n", msg);
        close(s_fd);
    }

    return EXIT_SUCCESS;
}
