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
#define CHILDNUMBER 50

char ip[] = "0.0.0.0";
int port = 9999;

int needprint = 0;

/* 子进程信息 */
typedef struct
{
    int is_use;         /* 是否使用中 0 or 1*/
    int child_num;      /* 子进程序号 */
    pid_t pid;          /* 子进程pid */
    int child_sock_fd;  /* 父子进程之间通信的unix域套接字描述符 */
    int state;          /* 子进程状态 0表示空闲，1表示忙 */
} child_t;

/* 清除变成僵尸进程的子进程 */
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

/* 向指定套接字发送文件描述符 */
ssize_t write_fd(int fd, void *ptr, size_t nbytes, int sendfd)
{
    struct msghdr msg;
    struct iovec iov[1];

    union {
        struct cmsghdr cm;
        char control[CMSG_SPACE(sizeof(int))];
    } control_un;
    struct cmsghdr *cmptr;

    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof(control_un.control);
    cmptr = CMSG_FIRSTHDR(&msg);
    cmptr->cmsg_len = CMSG_LEN(sizeof(int));
    cmptr->cmsg_level = SOL_SOCKET;
    cmptr->cmsg_type = SCM_RIGHTS;
    *((int *)CMSG_DATA(cmptr)) = sendfd;

    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    iov[0].iov_base = ptr;
    iov[0].iov_len = nbytes;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    return (sendmsg(fd, &msg, 0));
}

/* 从套接字接收文件描述符 */
ssize_t read_fd(int fd, void *ptr, size_t nbytes, int *recvfd)
{
    struct msghdr msg;
    struct iovec iov[1];
    ssize_t n;

    union {
        struct cmsghdr cm;
        char control[CMSG_SPACE(sizeof(int))];
    } control_un;
    struct cmsghdr *cmptr;

    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof(control_un);

    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    iov[0].iov_base = ptr;
    iov[0].iov_len = nbytes;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    if ((n = recvmsg(fd, &msg, 0)) <= 0)
        return n;

    if ((cmptr = CMSG_FIRSTHDR(&msg)) != NULL
        && cmptr->cmsg_len == CMSG_LEN(sizeof(int))) {
        *recvfd = *((int *) CMSG_DATA(cmptr));
    } else {
        *recvfd = -1;   /* 没有传文件描述符 */
    }

    return n;
}

/* 子进程的主函数 */
void child_main(int parent_sockfd, int child_num)
{
    
}

/* 子进程处理连接的具体事务 */
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
    /* 绑定SIGCHLD信号的处理函数 */
    signal(SIGCHLD, clean_child);

    /* 子进程使用变量 */
    int child_num;                      /* 子进程序号 父进程中赋值，在子进程中使用 */
    int parent_sockfd;                  /* 和父进程通信的unix域套接字描述符 */

    child_t  childs[CHILDNUMBER];       /* 子进程信息数组 */
    int childs_run_num = 0;             /* 当前正在运行中的子进程个数 */

    /* 预先创建一定个数的子进程，初始化子进程信息数组 */
    for (int i=0; i<CHILDNUMBER; i++) {
        child_num = i;                  /* 子进程记录下自己的序号 */
        childs[i].child_num = i;

        int sockfd[2];
        socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd);   /* 创建两个连接在一起的unix域套接字 */
        childs[i].child_sock_fd = sockfd[0];            /* 父进程使用sockfd[0]和子进程通信 */
        parent_sockfd = sockfd[1];                      /* 子进程使用sockfd[1]和父进程通信 */
        
        childs[i].is_use = 1;
        childs[i].state = 0;

        pid_t pid = fork();
        if (pid == 0) {                                 /* 子进程 */
            close(sockfd[0]);
            child_main(parent_sockfd, child_num);
            return 0;
        } else if (pid > 0) {                           /* 父进程 */
            close(sockfd[1]);
            childs[i].pid = pid;
        } else {
            printf("fork child %d error,%s\n", i, strerror(errno));
        }
    }

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
        /* 如果阻塞过程中被信号中断，某些系统可能会返回一个EINTR错误而不是重启accept */
        if (confd < 0 && errno == EINTR) {
            continue;
        } else if (confd < 0) {
            printf("accept error,%s\n", strerror(errno));
            return 0;
        }
        
        /* 在子进程信息数组中找到当前空闲的进程，通过unix域套接字传递和客户端的已连接套接字的描述符 */
        if (childs_run_num >= CHILDNUMBER) {
            close(confd);
            printf("当前处理进程繁忙，无法处理新连接！已自动关闭连接！\n");
            continue;
        }
        for (int i=0; i<CHILDNUMBER; i++) {
            if (childs[i].is_use == 1 && childs[i].state == 0) {
                char command = '1';     /* 发送数据为1表示是套接字描述符，0表示关闭当前子进程 */
                /* 向空闲的子进程发送已连接套接字描述符 */
                write_fd(childs[i].child_sock_fd, &command, 1, confd);
                close(confd);
                /* 更新这个子进程的状态 */
                childs[i].state = 1;
                childs_run_num++;
            }
        }
    }
    return 0;
}
