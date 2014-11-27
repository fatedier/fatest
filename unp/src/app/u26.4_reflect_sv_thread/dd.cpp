#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>

#define MAXLINE 1024

char ip[] = "0.0.0.0";
int port = 9999;

int needprint = 1;

/* 处理连接的具体事务 */
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
   
    close(sockfd);
}

/* 子线程处理函数 */
void *child_thread_main(void *ptr)
{
    pthread_detach(pthread_self());
    int recvfd, *p;
    p = (int *)ptr;
    recvfd = *p;
    free(p);
    if (needprint)
        printf("thread %lld：get socket %d\n", (long long)pthread_self(), recvfd);

    deal_socket(recvfd);
    if (needprint)
        printf("thread %lld：close socket %d\n", (long long)pthread_self(), recvfd);
    return NULL;
}

int main()
{
    int listenfd;

    struct sockaddr_in svaddr;
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

    struct sockaddr_in cliaddr;
    socklen_t len;
    len = sizeof(cliaddr);
    int *confd;
    for (;;) {
        confd = (int *)malloc(sizeof(int));
        *confd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
        /* 如果阻塞过程中被信号中断，某些系统可能会返回一个EINTR错误而不是重启accept */
        if (*confd < 0 && errno == EINTR) {
            free(confd);
            continue;
        } else if (*confd < 0) {
            printf("accept error,%s\n", strerror(errno));
            return 0;
        }
        
        if (needprint)
            printf("main:accept socket %d\n", *confd);

        pthread_t tid;
        pthread_create(&tid, NULL, child_thread_main, confd);
    }
    return 0;
}
