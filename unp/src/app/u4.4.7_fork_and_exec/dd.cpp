#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
    printf("the parent process: %d\n", getpid());

    pid_t pid;
    pid = fork();
    if(pid == 0)
    {
        printf("this is child: %d\n", getpid());
    }
    else if(pid > 0)
    {
        printf("this is parent: %d\n", getpid());
        char *argv[1];
        argv[0] = NULL;
        execv("./childs/child", argv);
    }
    else
    {
        printf("fork error!\n");
    }

    return 0;
}
