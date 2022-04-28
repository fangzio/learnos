#include "signal.h"
#include "stdio.h"
#include "sys/types.h"
#include "string.h"
// #include "sched.h"

void doSend(char *cmd, int pid, int sig);
void cmdInvalid(char *cmd);

int main()
{
    char cmd[10];
    int pid;
    while (1)
    {
        printf("please input your cmd\n");
        scanf("%s", cmd);
        setbuf(stdin, NULL);

        printf("please input target pid\n");
        scanf("%d", &pid);
        setbuf(stdin, NULL);
        if (strcmp(cmd, "tstp") == 0)
        {
            doSend(cmd, pid, SIGTSTP);
        }
        else if (strcmp(cmd, "cont") == 0)
        {
            doSend(cmd, pid, SIGCONT);
        }
        else
        {
            cmdInvalid(cmd);
        }
    }
}

void doSend(char *cmd, int pid, int sig)
{
    printf("send sig[%s] to pid[%d] \n", cmd, pid);
    kill(pid, sig);
}

void cmdInvalid(char *cmd)
{
    printf("your cmd[%s] is invalid,please input again \n", cmd);
}