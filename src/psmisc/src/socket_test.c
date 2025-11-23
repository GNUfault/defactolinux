/*
 * Creates named and anonymous sockets to test fuser
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, char *argv[])
{
    int skt;
    struct sockaddr_un name;

    if (argc < 2)
    {
        fprintf(stderr, "Usage:\n%s <socketpath>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ( (skt = socket(AF_UNIX, SOCK_SEQPACKET, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&name, 0, sizeof(name));

    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, argv[1], sizeof(name.sun_path) -1);
    
    if ( bind(skt, (const struct sockaddr *) &name, sizeof name) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if ( listen(skt, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ( accept(skt, NULL, NULL) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

//    sleep(100);
    exit(EXIT_SUCCESS);
}

