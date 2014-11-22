#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/select.h>

#define MAXLINE 1024
#define CLIENTNUM 1000

char ip[] = "127.0.0.1";
int port = 9999;

int max(int a, int b)
{
    return a>b?a:b;
}

//socket连接具体事务处理
void send_str(int sockfd)
{
    //最大描述符加1
    int maxfdp1;
    //控制是否已经读取到文件末尾，为0的时候才检查标准输入的可读性
    int io_flag = 0;
    //select集合
    fd_set rset;
    char sendline[MAXLINE], recvline[MAXLINE];

    FD_ZERO(&rset);
    for (;;) {
        if (io_flag == 0) {
            FD_SET(fileno(stdin), &rset);
        }
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(stdin), sockfd) + 1;
        select(maxfdp1, &rset, NULL, NULL, NULL);
        
        //标准输入描述符处于可读状态
        if (FD_ISSET(fileno(stdin), &rset)) {
            if (fgets(sendline, MAXLINE, stdin) == NULL) {
                io_flag = 1;
                shutdown(sockfd, SHUT_WR);
                FD_CLR(fileno(stdin), &rset);
                continue;
            } else {
                write(sockfd, sendline, strlen(sendline));
            }
        }

        //socket描述符处于可读状态
        if (FD_ISSET(sockfd, &rset)) {
            int n = read(sockfd, recvline, MAXLINE);
            if (n < 0) {
                printf("read error\n");
                return;
            } else if (n == 0) {
                //收到FIN信号，如果之前标准输入已经读取结束了，正常关闭，否则说明出错
                if (io_flag == 1) {
                    return;
                } else {
                    printf("the socket is closed\n");
                    return;
                }
            }
            fputs(recvline, stdout);
            memset(&recvline, 0 , sizeof(recvline));  
        }
    }
}

int main()
{
    //不响应信号SIGPIPE
    signal(SIGPIPE, SIG_IGN);

    int confd[CLIENTNUM];

    for (int i=0; i<CLIENTNUM; i++) {
        confd[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (confd[i] < 0) {
            printf("create error,%s\n", strerror(errno));
            return 0;
        }

        struct sockaddr_in cliaddr;
        memset(&cliaddr, 0, sizeof(cliaddr));
        cliaddr.sin_family = AF_INET;
        cliaddr.sin_addr.s_addr = inet_addr(ip);
        cliaddr.sin_port = htons(port);

        if (connect(confd[i], (struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0) {
            printf("connect error,%s\n", strerror(errno));
            return 0;
        }
    }

    send_str(confd[0]);
    
    return 0;
}
