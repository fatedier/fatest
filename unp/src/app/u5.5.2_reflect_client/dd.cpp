#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#define MAXLINE 1024

//socket连接具体事务处理
void send_str(int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];
    while (fgets(sendline, sizeof(sendline), stdin) != NULL) {
        write(sockfd, sendline, strlen(sendline));
        //如果之前服务器主动关闭了连接，客户端阻塞在fgets的文件描述符上，还没向对方发送FIN
        //这时候再发送数据会收到一个RST，再次发送的话就会接收到一个SIGPIPE信号并终止
        //这里忽略该信号，并根据errno来判断
        if (errno == EPIPE) {
            printf("socket is closed,can not send this msg\n");
            return;
        }
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
    //不响应信号SIGPIPE
    signal(SIGPIPE, SIG_IGN);

    int confd[1];

    for (int i=0; i<1; i++) {
        confd[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (confd[i] < 0) {
            printf("create error,%s\n", strerror(errno));
            return 0;
        }

        struct sockaddr_in cliaddr;
        memset(&cliaddr, 0, sizeof(cliaddr));
        cliaddr.sin_family = AF_INET;
        cliaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        cliaddr.sin_port = htons(9999);

        if (connect(confd[i], (struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0) {
            printf("connect error,%s\n", strerror(errno));
            return 0;
        }
    }

    send_str(confd[0]);
    
    return 0;
}
