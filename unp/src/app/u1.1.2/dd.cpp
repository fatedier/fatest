#include <stdio.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#define MAXLINE 1024

int main(int argc, char **argv)
{
    int sockfd;
    int n;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;

    if (argc != 2)
    {
        printf("usage:a.out <IPaddress>\n");
        return 1;
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socet error\n");
        return 1;
    }
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        printf("inet_pton error for %s\n", argv[1]);
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    {
        printf("connect error\n");
        printf("%d\n", errno);
        return 1;
    }

    int count = 0;
    while ((n = read(sockfd, recvline, MAXLINE)) > 0)
    {
        count++;
        recvline[n] = 0;
        if (fputs(recvline, stdout) == EOF)
        {
            printf("fputs error\n");
            return 1;
        }
    }

    if (n < 0)
    {
        printf("read error\n");
        return 1;
    }
    printf("%d\n", count);
    return 0;
}
