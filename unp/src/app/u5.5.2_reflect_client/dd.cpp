#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 1024

void send_str(int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];
    while (fgets(sendline, sizeof(sendline), stdin) != NULL) {
        write(sockfd, sendline, strlen(sendline));
        if (read(sockfd, recvline, MAXLINE) < 0) {
            printf("read error\n");
            return;
        }
        fputs(recvline, stdout);
        memset(&recvline, 0 , sizeof(recvline));
    }
}

int main()
{
    int confd;
    confd = socket(AF_INET, SOCK_STREAM, 0);
    if (confd < 0) {
        printf("create error,%s\n", strerror(errno));
        return 0;
    }

    struct sockaddr_in cliaddr;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    cliaddr.sin_port = htons(9999);

    if (connect(confd, (struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0) {
        printf("connect error,%s\n", strerror(errno));
        return 0;
    }

    send_str(confd);
    
    return 0;
}
