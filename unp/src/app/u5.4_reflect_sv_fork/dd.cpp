#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#define MAXLINE 1024

char ip[] = "0.0.0.0";
int port = 9999;

int needprint = 0;

//清除变成僵尸进程的子进程
void clean_child(int signo)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        if (needprint)
            printf("child %d terminated\n", pid);
    }
    return;
}

//处理连接的具体事务
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
    //绑定SIGCHLD信号的处理函数
    signal(SIGCHLD, clean_child);

    int listenfd, confd;

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

    pid_t pid;
    struct sockaddr_in cliaddr;
    socklen_t len;
    len = sizeof(cliaddr);
    for (;;) {
        confd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
        //如果阻塞过程中被信号中断，某些系统可能会返回一个EINTR错误而不是重启accept
        if (confd < 0 && errno == EINTR) {
            continue;
        } else if (confd < 0) {
            printf("accept error,%s\n", strerror(errno));
            return 0;
        }

        pid = fork();
        //子进程
        if (pid == 0) {
            close(listenfd);
            deal_socket(confd);
            close(confd);
            return 0;
        } else if (pid > 0) {
        //父进程
            close(confd);
        } else {
            printf("fork error\n");
        }
    }
    return 0;
}
