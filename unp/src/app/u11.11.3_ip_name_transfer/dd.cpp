#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    char **pptr;
    struct hostent *hptr;
    
    if (argc != 2) {
        printf("Usage:\n");
        printf("    xxxx [host_name]\n");
        return -1;
    }

    hptr = gethostbyname(argv[1]);
    if (hptr == NULL) {
        printf("error: %s\n", strerror(errno));
        return -1;
    }

    printf("official hostname: %s\n", hptr->h_name);
    
    for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
        printf("         alias: %s\n", *pptr);

    struct in_addr address;
    for (pptr = hptr->h_addr_list; *pptr != NULL; pptr++) {
        memcpy(&address, *pptr, hptr->h_length);
        printf("         address: %s\n", inet_ntoa(address));
    }

    return 0;
}
