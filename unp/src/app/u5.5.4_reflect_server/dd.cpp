#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define MAXLINE 1024

//清除变成僵尸进程的子进程
void clean_child(int signo)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        printf("child %d terminated\n", pid);
    }
    return;
}

//处理连接的具体事务
void deal_socket(int sockfd)
{
    char buf[MAXLINE];
    int n;
    long arg1,arg2;
again:
    while ((n = read(sockfd, buf, MAXLINE)) > 0) {
        if (sscanf(buf, "%ld%ld", &arg1, &arg2) == 2)
            snprintf(buf, sizeof(buf), "%ld\n", arg1 + arg2);
        else
            snprintf(buf, sizeof(buf), "input error\n");
        n = strlen(buf);
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
