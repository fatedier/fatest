#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MSG_KEY 0x1
#define MAX_DATA 1024

typedef struct
{
    long ntype;
    char data[MAX_DATA];
} msg_data_t;

void show_help()
{
    printf("Usage: -s [command] -c [content]\n");
    printf("    -s    use create,snd,rcv,close to start with different function\n");
    printf("    -c    with \"-s snd\" set the send message content\n");
}

int main(int argc, char **argv)
{
    char start_cmd[20];
    memset(start_cmd, 0, sizeof(start_cmd));
    char buf[MAX_DATA];
    memset(buf, 0, sizeof(buf));

    int c;
    while ((c = getopt(argc, argv, "s:c:")) != -1) {
        switch (c) {
        case 's':
            strncpy(start_cmd, optarg, sizeof(start_cmd));
            break;
        
        case 'c':
            strncpy(buf, optarg, sizeof(buf));
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
    }
    
    /* 创建一个消息队列 */
    if (strcmp(start_cmd, "create") == 0) {
        int msqid = msgget(MSG_KEY, IPC_CREAT | 0664);
        if (msqid < 0)
            perror("msgget error:");
    } else if (strcmp(start_cmd, "snd") == 0) {
    /* 向消息队列中发送一条消息 */
        int msqid = msgget(MSG_KEY, 0);
        if (msqid < 0) {
            perror("msgget error:");
            return -1;
        }
        
        if (buf[0] == 0) {
            printf("error: please use -c option to set send msg content\n");
            return -1;
        }

        msg_data_t msg;
        msg.ntype = 1;
        strncpy(msg.data, buf, sizeof(msg.data));
        msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0);
    } else if (strcmp(start_cmd, "rcv") == 0) {
    /* 从消息队列中接收一条消息 */
        int msqid = msgget(MSG_KEY, 0);
        if (msqid < 0) {
            perror("msgget error:");
            return -1;
        }

        msg_data_t msg;
        int n = msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), 0, 0);
        printf("read %d bytes,type = %ld content = %s\n", n, msg.ntype, msg.data);
    } else if (strcmp(start_cmd, "close") == 0) {
        int msqid = msgget(MSG_KEY, 0);
        if (msqid < 0) {
            perror("msgget error:");
            return -1;
        }
        
        msgctl(msqid, IPC_RMID, NULL);
    } else {
        printf("error: -s option can not support the command \"%s\"\n", start_cmd);
        return -1;
    }

    return 0;
}
