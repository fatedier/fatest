#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define MAX_DATA 1024
#define SHM_KEY 0x1

void show_help()
{
    printf("Usage: -s [command] -c [content]\n");
    printf("    -s    use create,write,read,close to start with different function\n");
    printf("    -c    with \"-s write\" set put object in share memory content\n");
}

int main(int argc, char **argv)
{
    char start_cmd[20] = {0};       /* 启动命令 */
    char content[MAX_DATA] = {0};   /* 内容字符串 */
    
    char *ptr;  /* 共享内存空间地址 */
    
    int c;
    while ((c = getopt(argc, argv, "s:c:")) != -1) {
        switch (c) {
        case 's':
            strncpy(start_cmd, optarg, sizeof(start_cmd));
            break;

        case 'c':
            strncpy(content, optarg, sizeof(content));
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

    if (strcmp(start_cmd, "") == 0) {
        printf("error: please use -s option to set start_cmd\n");
        return -1;
    } else if (strcmp(start_cmd, "create") == 0) {
    /* 创建共享内存 */
        int shm_id = shmget(SHM_KEY, MAX_DATA, 0640 | IPC_CREAT);
        ptr = shmat(shm_id, NULL, 0);
    } else if (strcmp(start_cmd, "close") == 0) {
    /* 关闭共享内存 */
        int shm_id = shmget(SHM_KEY, 0, 0640 | IPC_CREAT);
        shmctl(shm_id, IPC_RMID, NULL);
    } else if (strcmp(start_cmd, "write") == 0) {
    /* 向共享内存中写入数据 */
        int shm_id = shmget(SHM_KEY, 0, 0640 | IPC_CREAT);
        ptr = shmat(shm_id, NULL, 0);
        memcpy(ptr, content, strlen(content) + 1);    
    } else if (strcmp(start_cmd, "read") == 0) {
    /* 从共享内存中读出数据 */
        int shm_id = shmget(SHM_KEY, 0, 0640 | IPC_CREAT);
        ptr = shmat(shm_id, NULL, 0);
        //memcpy(content, ptr, sizeof(content));
        printf("%s\n", ptr);
    }
    return 0;
}
