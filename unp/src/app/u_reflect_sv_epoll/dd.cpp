#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <malloc.h>

#define MAXLINE 1024
#define MAXCLIENT 5000

char ip[] = "0.0.0.0";
int port = 9999;

int needPrint = 1;

struct my_epoll_data_t
{
    char data[MAXLINE];
    int len;
    int fd;
};

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
    int nReady;                     //每次epoll返回后就绪的描述符个数
    char buf[MAXLINE];
    int nFdUse = 0;
    struct sockaddr_in cliaddr;
    socklen_t len;

    int epoll_fd;
    epoll_fd = epoll_create(MAXCLIENT);
    struct epoll_event event;
    struct epoll_event events[MAXCLIENT];
    event.data.fd = listenfd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listenfd, &event);
    for (;;) {
        nReady = epoll_wait(epoll_fd, events, MAXCLIENT, -1);
        for (int i=0; i<nReady; i++) {
            //监听套接字处理
            if (events[i].data.fd == listenfd) {
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
                        event.data.fd = confd;
                        event.events = EPOLLIN | EPOLLET;
                        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, confd, &event);
                        nFdUse++;
                    }
                }
            } else if (events[i].events & EPOLLIN) {
                //遍历与客户端连接的套接字的处理
                int n;
                if ((n = read(events[i].data.fd, buf, MAXLINE)) < 0) {
                    printf("read error\n");
                    event.data.fd = events[i].data.fd;
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &event);
                    close(events[i].data.fd);
                    nFdUse--;

                } else if (n == 0) {
                    if (needPrint)
                        printf("remote socket is closed\n");
                    event.data.fd = events[i].data.fd;
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &event);
                    close(events[i].data.fd);
                    nFdUse--;
                }
                else {
                    //将该套接字改为监听可写状态
                    struct my_epoll_data_t *md;
                    md = (struct my_epoll_data_t *)malloc(sizeof(struct my_epoll_data_t));
                    strncpy(md->data, buf, n);
                    md->len = n;
                    md->fd = events[i].data.fd;
                    event.data.ptr = md;
                    event.events = EPOLLOUT | EPOLLET;
                    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, events[i].data.fd, &event);
                }
            } else if (events[i].events & EPOLLOUT) {
                //套接字可写，回射客户端发过来的消息
                struct my_epoll_data_t *md = (struct my_epoll_data_t *)events[i].data.ptr;
                write(md->fd, md->data, md->len);
                event.data.fd = md->fd;
                event.events = EPOLLIN | EPOLLET;
                free(md);
                epoll_ctl(epoll_fd, EPOLL_CTL_MOD, event.data.fd, &event);
            }
        }
    }
    return 0;
}
