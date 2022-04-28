#include "mytool.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"

void mysay();
void mytest();

void main() { mysay(); }

void mysay() {
    int clientfd;
    char *hostname = "localhost";
    char *port = "9090";
    char net_buf[MYMSG_SIZE];
    char lc_buf[MYMSG_SIZE];
    int nums;
    clientfd = open_clientfd(hostname, port);
    memset(lc_buf, 0, MYMSG_SIZE);
    // 从stdin读取数据
    while (fgets(lc_buf, MYMSG_SIZE, stdin) != NULL) {
        // 写入fd
        nums = write(clientfd, lc_buf, strlen(lc_buf));
        // printf("strlen(lc_buf) is [%d]bytes,write to server [%d]bytes.\n", strlen(lc_buf), nums);
        memset(net_buf, 0, MYMSG_SIZE);
        // 从fd读取数据
        nums = my_readall(clientfd, net_buf, MYMSG_SIZE);
        // printf("strlen(net_buf) is [%d]bytes,read from server [%d]bytes.\n", strlen(net_buf), nums);
        // 输出到stdout
        sprintf(net_buf, "res is:%s", strdup(net_buf));
        fputs(net_buf, stdout);
        memset(lc_buf, 0, MYMSG_SIZE);
    }
    // 0123456789abcdefghigk
    close(clientfd);
    printf("client done.\n");
    exit(0);
}

void mytest() {
    char buf[MYMSG_SIZE];
    my_readall(0, buf, MYMSG_SIZE);
    printf("input is %s\n", buf);
}