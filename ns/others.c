#define _GNU_SOURCE
#include "sched.h"
#include "stdio.h"
#include "unistd.h"
#include "signal.h"
#include "sys/wait.h"
#include "sys/mount.h"
#include "string.h"

// 使用linux测试，mac没有clone函数
// 定义一个给container用的stack,1M
#define CNT_STACK (1024 * 1024)
static char container_stack[CNT_STACK];

int container_main(void *arg);

int main()
{
    printf("host is begin,pid:[%d]\n", getpid());
    // 调用clone函数
    int container_id = clone(container_main, container_stack + CNT_STACK, CLONE_NEWUTS | CLONE_NEWPID | CLONE_NEWNS | SIGCHLD, NULL);
    printf("container_id is %d\n", container_id);
    // 等待子进程结束
    waitpid(container_id, NULL, 0);
    printf("host:container is end\n");
    return 0;
}

char *const container_args[] = {
    "/bin/bash",
    NULL,
};

int container_main(void *arg)
{
    const char *container_name = "zzfcontainer";
    printf("container is begin,pid:[%d]\n", getpid());
    mount("proc", "/proc", "proc", 0, NULL);
    // 设置hostname
    if (sethostname(container_name, strlen(container_name)) == -1)
    {
        printf("set container hostname error");
    }
    // 执行bash
    execv(container_args[0], container_args);
    printf("container execv is err\n");
    return 1;
}