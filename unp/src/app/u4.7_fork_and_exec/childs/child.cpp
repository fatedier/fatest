#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("the new process: %d\n", getpid());
    return 0;
}
