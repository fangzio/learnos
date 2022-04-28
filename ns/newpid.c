#define _GNU_SOURCE
#include "sched.h"
#include "stdio.h"
#include "stdlib.h"
#include "signal.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "time.h"
#include "unistd.h"
#include "string.h"
#include "sys/mount.h"

#define STACK_SIZE 1024 * 1024 * 10
#define PARENT_WAIT 3 // 子进程退出后，父进程退出前等待的时间

int container_main(void *arg);
void stopProc(int signo);
void doExit(int signo);

typedef struct pNode
{
    int pid;
    struct pNode *Next;
} prcNode;

prcNode *pArr;

int main()
{
    printf("host begin\n");
    signal(SIGINT, stopProc);
    signal(SIGTERM, stopProc);
    signal(SIGTSTP, stopProc);
    signal(SIGSTOP, stopProc);
    signal(SIGKILL, stopProc);
    // 记得分配内存
    pArr = (prcNode *)malloc(sizeof(prcNode));

    void *pstk = malloc(STACK_SIZE);
    int newPid = clone(container_main, (char *)pstk + STACK_SIZE, CLONE_NEWUTS | CLONE_NEWPID | CLONE_NEWNS | SIGCHLD, NULL);
    if (newPid == -1)
    {
        perror("clone faild");
    }
    else
    {
        pArr->pid = newPid;
    }

    int status;
    printf("wait for pid:%d\n", newPid);
    if (waitpid(newPid, &status, 0) == -1)
    {
        perror("waitpid err");
    }
    printf("child exist,status:[%d],parent will exist after [%d]s\n", status, PARENT_WAIT);
    sleep(PARENT_WAIT);
    printf("host end\n");
}

int container_main(void *arg)
{
    signal(SIGINT, doExit);
    printf("container start\n");
    // 设置容器名称
    char *name = "zzfcontainer";
    sethostname(name, strlen(name));
    // 挂载文件系统
    mount("proc", "/proc", "proc", 0, NULL);
    while (1)
    {
        time_t timep;
        struct tm *p;
        time(&timep);
        p = gmtime(&timep);
        // printf("[%d:%d:%d] -- ok\n", p->tm_hour, p->tm_min, p->tm_sec);
        sleep(1);
    }
    printf("container end\n");
}

void stopProc(int signo)
{
    prcNode *tp = pArr;
    printf("receive signo:[%d]\n", signo);
    while (tp != NULL && tp->pid > 0)
    {
        printf("before term pid:[%d]\n", tp->pid);
        int rs = kill(tp->pid, SIGINT);
        printf("term pid:[%d],rs:[%d]\n", tp->pid, rs);
        tp = tp->Next;
        usleep(100);
    }
}

void doExit(int signo)
{
    printf("receive signo:[%d]\n", signo);
    exit(0);
}