#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 1024

void deal_socket(int sockfd)
{
    char buf[MAXLINE];
    int n;
again:
    while ((n = read(sockfd, buf, MAXLINE)) > 0) {
        write(sockfd, buf, n);
    }

    if (n < 0 && errno == EINTR)
        goto again;
    else if (n < 0)
        printf("read error\n");
}

int main()
{
    int listenfd, confd;

    struct sockaddr_in svaddr;
    svaddr.sin_family = AF_INET;
    svaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    svaddr.sin_port = htons(9999);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        printf("create error,%s\n", strerror(errno));
        return 0;
    }
    
    if (bind(listenfd, (struct sockaddr *)&svaddr, sizeof(svaddr)) < 0) {
        printf("bind error,%s\n", strerror(errno));
        return 0;
    }

    if (listen(listenfd, 50) < 0) {
        printf("listen error,%s\n", strerror(errno));
        return 0;
    }

    pid_t pid;
    struct sockaddr_in cliaddr;
    socklen_t len;
    len = sizeof(cliaddr);
    for (;;) {
        confd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
        if (confd < 0) {
            printf("accept error,%s\n", strerror(errno));
            return 0;
        }

        pid = fork();
        if (pid == 0) {
            close(listenfd);
            deal_socket(confd);
            close(confd);
            return 0;
        } else if (pid > 0) {
            close(confd);
        } else {
            printf("fork error\n");
        }
    }
    return 0;
}
