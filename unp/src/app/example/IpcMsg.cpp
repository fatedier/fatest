#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <wait.h>
using namespace std;

int key = 1001;
int msgid;

struct msgType
{
    long nType;
    long text;
};

int main()
{
    cout<<"Hello!"<<endl;
    //key = ftok(".", 1);
    if((msgid = msgget(key, IPC_CREAT|0660)) == -1)
    {
        cout<<"Create Msg Error!"<<endl;
        return 0;
    }
    cout<<"key = "<<key<<endl;
    cout<<"msgid = "<<msgid<<endl;

    pid_t pid;
    pid = fork();
    if(pid > 0)
    {
        cout<<"Main process: "<<endl;
        cout<<"Main pid: "<< getpid() <<endl;
        msgType msg;
        msg.nType = 1;
        msg.text = 0;
        for(int i=0; i<5; i++)
        {
            msg.text = i;
            if(-1 != msgsnd(msgid, &msg, sizeof(struct msgType)-sizeof(long), 0))
            {
                cout<<"Msg send "<< i <<" : "<<msg.text<<endl;
            }
            else
            {
                cout<<"Send msg error!"<<endl;
            }
        }
        wait(0);
    }
    else if(pid == 0)
    {
        cout<<"Child process: "<<endl;
        cout<<"Child pid: "<< getpid() <<endl;
        msgType msg;
        int i = 0;
        while(true)
        {
            if(i >= 5)
            {
                break;
            }
            if(-1 != msgrcv(msgid, &msg, sizeof(struct msgType)-sizeof(long), 1, IPC_NOWAIT))
            {
                cout<<"Msg receive "<< i << " : "<< msg.text <<endl;
                i++;
            }
            else
            {
                cout<<"No msg get!Sleep 1 sec!"<<endl;
                sleep(1);
            }
        }
    }
    else
    {
        cout<<"Fork error!"<<endl;
    }
    return 0;
}
