#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "chat_socket.h"

#ifndef true
typedef char bool;
#define true 1
#define false 0
#endif

#define MAX_USER_NAME 20
#define MAX_BUFF 2048

void show_help()
{
    printf("Usage: [-s | -c] -u [username] [ip port]\n");
    printf("    -s as chat server, port are needed\n");
    printf("    -c as chat client, ip and port are needed\n");
    printf("    -u set the username when you chat to others\n");
}

void *deal_read(void *sockfd)
{
    int *p;
    p = (int *)sockfd;
    int confd = *p;
    free(p);

    int n;
    char buff[MAX_BUFF] = {0};
    char *username;

again:
    while ((n = read(confd, buff, MAX_BUFF)) > 0) {
        if (n <= MAX_USER_NAME) {
            printf("read error,receive date not enough\n");
            continue;
        }
        
        username = buff;
        int i;
        for (i=0; i<MAX_USER_NAME; i++) {
            if (buff[i] == ' ') {
                buff[i] = '\0';
                break;
            }
        }
        char *p = buff + MAX_USER_NAME;
        for (i=MAX_USER_NAME; i<n; i++) {
            if (buff[i] == '\n') {
                buff[i] = '\0';
                break;
            }
        }
        printf("%s : %s\n", username, p);
    }
    
    if (n < 0 && errno == EINTR)
        goto again;
    else if (n < 0)
        printf("read error, %s\n", strerror(errno));

    close(confd);
    return NULL;
}

void *deal_write(void *sockfd)
{
    int *p;
    p = (int *)sockfd;
    int confd = *p;
    free(p);

    return NULL;
}

int main(int argc, char **argv)
{
    bool is_server = true;
    char ip[16] = {0};
    int port = 0;
    char username[MAX_USER_NAME] = {0};

    int c;
    while ((c = getopt(argc, argv, "scu:")) != -1) {
        switch (c) {
        case 's':
            is_server = true;
            port = atol(argv[argc - 1]);
            break;

        case 'c':
            is_server = false;
            strncpy(ip, argv[argc - 2], sizeof(ip));
            port = atol(argv[argc - 1]);
            break;
        
        case 'u':
            strncpy(username, optarg, sizeof(username));
            break;

        case '?':
            show_help();
            return -1; 
        }
    }

    if (optind == 1) {
        show_help();
        return -1;
    }
    
    int listenfd;
    int *confd;
    int *confd2;
    /* server */
    if (is_server) {
        listenfd = set_socket_server(ip, port);
        if (listenfd < 0) {
            printf("set socket server error: %s\n", strerror(errno));
            return -1;
        }
        printf("wait for others to connect...\n");

        struct sockaddr_in cliaddr;
        socklen_t len;
        len = sizeof(cliaddr);
        confd = (int *)malloc(sizeof(int));
        confd2 = (int *)malloc(sizeof(int));
        if ((*confd = accept(listenfd, (struct sockaddr *)&cliaddr, &len)) < 0) {
            printf("accept error: %s\n", strerror(errno));
            return -1;
        }
        *confd2 = *confd;
        printf("accept a connect\n");
        pthread_t tid, tid2;
        pthread_create(&tid, NULL, deal_read, confd);
        pthread_create(&tid2, NULL, deal_write, confd2);
        pthread_join(tid, NULL);
        pthread_join(tid2, NULL);
    } else {
    /* client */
        
    }

    return 0;
}
