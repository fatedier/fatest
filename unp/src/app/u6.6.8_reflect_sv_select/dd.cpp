#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/select.h>

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

int main()
{
    //绑定SIGCHLD信号的处理函数
    signal(SIGCHLD, clean_child);

    int listenfd;

    struct sockaddr_in svaddr;
    memset(&svaddr, 0, sizeof(svaddr));
    svaddr.sin_family = AF_INET;
    svaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    svaddr.sin_port = htons(9999);

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

    int client[FD_SETSIZE];         //存放处于连接状态的套接字，其余元素为-1
    int nUserful = 0;               //client数组中有效连接的个数
    int confd;
    int maxfd1 = listenfd + 1;      //最大文件描述符加1
    fd_set rset,allset;             //读描述符集合,allset用于持久化存储所有的状态，每次循环同步到rset
    int nReady;                     //每次select返回后就绪的描述符个数
    char buf[MAXLINE];

    for (int i=0; i<FD_SETSIZE; i++) {
        client[i] = -1;
    }

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    struct sockaddr_in cliaddr;
    socklen_t len;
    len = sizeof(cliaddr);
    for (;;) {
        rset = allset;
        
        nReady = select(maxfd1, &rset, NULL, NULL, NULL);
        
        if (FD_ISSET(listenfd, &rset)) {
            confd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
            if (confd < 0) {
                printf("accept error,%s\n", strerror(errno));
                return 0;
            }
            printf("accept sockfd:%d allnumber:%d\n", confd, nUserful + 1);
            //将接收到的套接字描述符加入到client数组中，第一个<0的位置
            int i;
            for (i=0; i<FD_SETSIZE; i++) {
                if (client[i] < 0) {
                    client[i] = confd;
                    break;
                }
            }
            //如果client数组满了，连接数已经超过上限，不能继续连接
            if (i == FD_SETSIZE) {
                printf("too many clients\n");
                close(confd);
            } else {
                //有效连接数加1，将套接字描述符加入allset，修改maxfd1
                nUserful++;
                FD_SET(confd, &allset);
                if (maxfd1 < confd + 1)
                    maxfd1 = confd + 1;
            }
            //如果没有其他文件描述符就绪了，直接跳过
            if (--nReady <= 0)
                continue;
        }

        int nDeal = 0;  //用来计数已经处理了多少个有效连接，配合nUserful使用
        for (int i=0; i<FD_SETSIZE; i++) {
            if (client[i] >= 0) {
                nDeal++;
                if (FD_ISSET(client[i], &rset)) {
                    //printf("sockfd [%d] is already\n", client[i]);
                    int n;
                    //read返回0的时候，表示对方已经关闭连接
                    if ((n = read(client[i], buf, MAXLINE)) == 0) {
                        printf("remote socket is closed\n");
                        close(client[i]);
                        FD_CLR(client[i], &allset);
                        client[i] = -1;
                        nUserful--;
                    } else if (n < 0) {
                        //返回小于0表示发生错误
                        printf("connection:%d read error\n", client[i]);
                        close(client[i]);
                        FD_CLR(client[i], &allset);
                        client[i] = -1;
                        nUserful--;
                    } else {
                        //printf("%s\n", buf);
                        write(client[i], buf, n);
                    }
                    //处理完这条，如果没有其他文件描述符就绪，直接跳过
                    if (--nReady <= 0)
                        break;
                }
            }
            if (nDeal >= nUserful)
                break;
        }
    }
    return 0;
}
