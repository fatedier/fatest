#include <stdio.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

#define MAXLINE 1024

int main(int argc, char **argv)
{
    int listenfd;
    int connfd;
    char buff[MAXLINE + 1];
    struct sockaddr_in servaddr;
    time_t ticks;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socet error\n");
        return 1;
    }
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13);

    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    {
        printf("bind error\n");
        return 1;
    }

    if (listen(listenfd, 50) < 0)
    {
        printf("listen error\n");
        return 1;
    }
    
    for(;;)
    {
        connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);
        if (connfd < 0)
        {
            printf("accept error\n");
            return 1;
        }
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

        for (int i=0; i<strlen(buff); i++)
        {
            if (write(connfd, &buff[i], 1) < 0)
            {
                printf("write error\n");
            }

        }

        if (close(connfd) < 0)
        {
            printf("close error\n");
        }
    }
  
    return 0;
}
