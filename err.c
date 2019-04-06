#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void check_err(int i) 
{
    if(i < 0) 
    {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }
}
