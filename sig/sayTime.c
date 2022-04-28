#include "stdio.h"
#include "time.h"
#include "unistd.h"
#include "signal.h"
#include "stdlib.h"

void onContinue();

int cCount = 0;

int main()
{
    time_t nowptr;
    struct tm *nowTime;
    // 注册接收到sigcont的处理器
    signal(SIGCONT, onContinue);

    while (1)
    {
        nowptr = time(NULL);
        nowTime = gmtime(&nowptr);
        printf("time is [%d:%d:%d]\n", nowTime->tm_hour, nowTime->tm_min, nowTime->tm_sec);
        sleep(1);
    }
}

void onContinue()
{
    cCount++;
    if (cCount > 1)
    {
        printf("you have restart 1 times, we will exit.\n");
        exit(0);
    }
    printf("restart times:[%d]\n", cCount);
}