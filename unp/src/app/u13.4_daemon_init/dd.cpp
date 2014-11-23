#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_FD 1024

int daemon_init()
{
    pid_t pid;
    
    if ((pid = fork()) < 0)
        return -1;
    else if (pid > 0)
        exit(0);        /* 结束父进程 */
    
    /* 第一次fork后的子进程 */
    if (setsid() < 0)   /* 成为新的会话的领头进程 */
        return -1;

    signal(SIGHUP, SIG_IGN);
    if ((pid = fork()) < 0)
        return -1;
    else if (pid > 0)
        exit(0);       /* 结束第一次fork后的子进程 */

    /* 第二次fork后的子进程 */
    chdir("/");        /* 修改当前工作目录 */

    /* 关闭所有文件描述符 */
    for (int i=0; i<MAX_FD; i++)
        close(i);

    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);

    return 0;
}

int main(int argc, char **argv)
{
    if (daemon_init() < 0)
        printf("error!");
    sleep(20);
    return 0;
}
