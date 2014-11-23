#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <poll.h>
#include <arpa/inet.h>

#define MAXLINE 1024
#define MAXCLIENT 1024

char ip[] = "0.0.0.0";
int port = 9999;

int needPrint = 1;

int main()
{
    int listenfd;

    struct sockaddr_in svaddr;
    memset(&svaddr, 0, sizeof(svaddr));
    svaddr.sin_family = AF_INET;
    svaddr.sin_addr.s_addr = inet_addr(ip);
    svaddr.sin_port = htons(port);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        printf("create error,%s\n", strerror(errno));
        return 0;
    }
    
    int reuse = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        printf("setsockopt error,%s\n", strerror(errno));
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

    int confd;
    int nReady;                     //每次poll返回后就绪的描述符个数
    char buf[MAXLINE];
    struct pollfd client[MAXCLIENT];     //pollfd结构体数组
    int nFdUse = 0;                 //pollfd数组中有效连接的个数
    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    nFdUse++;

    for (int i=1; i<MAXCLIENT; i++) {
        client[i].fd = -1;
    }

    struct sockaddr_in cliaddr;
    socklen_t len;
    for (;;) {
        nReady = poll(client, MAXCLIENT, -1);
        //监听套接字处理
        if (client[0].revents & POLLRDNORM) {
            len = sizeof(cliaddr);
            confd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
            if (confd < 0) {
                printf("accept error,%s\n", strerror(errno));
            } else {
                if (needPrint)
                    printf("accept sockfd:%d allnumber:%d\n", confd, nFdUse + 1);

                if (nFdUse + 1 > MAXCLIENT) {
                    if (needPrint)
                        printf("too many clients!!!\n");
                    close(confd);
                } else {
                    for (int i=0; i<MAXCLIENT; i++) {
                        if (client[i].fd < 0) {
                            client[i].fd = confd;
                            client[i].events = POLLRDNORM;
                            nFdUse++;
                            break;
                        }
                    }
                }
            }

            if (--nReady <= 0)
                continue;
        }

        //遍历与客户端连接的套接字的处理
        for (int i=1; i<MAXCLIENT; i++) {
            if (client[i].fd < 0)
                continue;
            if (client[i].revents & (POLLRDNORM | POLL_ERR)) {
                int n;
                if ((n = read(client[i].fd, buf, MAXLINE)) < 0) {
                    printf("read error\n");
                    close(client[i].fd);
                    client[i].fd = -1;
                    nFdUse--;

                } else if (n == 0) {
                    if (needPrint)
                        printf("remote socket is closed\n");
                    close(client[i].fd);
                    client[i].fd = -1;
                    nFdUse--;
                }
                else {
                    write(client[i].fd, buf, n);
                }

                if (--nReady <= 0)
                    break;
            }
        }
    }
    return 0;
}
