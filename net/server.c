#include "mytool.h"
#include "netdb.h"
#include "pthread.h"
#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/select.h"
#include "sys/socket.h"
#include "unistd.h"

void mylisten();
void mylisten_iomulti();
void mylisten_multithread();
void *my_thread_handler(void *argv);
void onMyInt(int signo);
void mySay(int fd);
void command();

void main() {
    signal(SIGINT, onMyInt);
    // 监听
    mylisten_multithread();
}

/**
 * 多线程
 */
void mylisten_multithread() {
    // 监听指定端口，获取fd
    int listenfd = open_listenfd("9090");
    struct sockaddr_storage client_addr;
    int clientlen;
    clientlen = sizeof(struct sockaddr_storage);
    pthread_t tid;
    int rc; // 错误码
    while (1) {
        // 接收一个socket连接，拿到连接fd
        int connfd = accept(listenfd, (struct sockaddr *)&client_addr, &clientlen);
        if (connfd > 0) {
            // 创建线程，处理接收到的客户端连接
            pthread_create(&tid, NULL, my_thread_handler, (void *)&connfd);
        } else {
            perror("accept err");
            exit(0);
        }
    }
}

void *my_thread_handler(void *argv) {
    int fd;
    fd = *(int *)argv;
    printf("new thread for fd:[%d]\n", fd);
    while (1) {
        mySay(fd);
    }
}

/**
 * IO多路复用
 */
void mylisten_iomulti() {
    int listenfd, connfd;
    struct sockaddr_storage clientaddr;
    socklen_t clientlen;
    fd_set ready_set, read_set;
    char client_hostname[MYNAME_SIZE], client_port[MYNAME_SIZE];
    // 监听，并返回fd
    listenfd = open_listenfd("9090");
    // 清空可读fd集合
    FD_ZERO(&read_set);
    // 添加stdin到可读集合
    FD_SET(STDIN_FILENO, &read_set);
    // 添加监听fd到可读集合
    FD_SET(listenfd, &read_set);
    // 初始化存储连接fd的链表
    struct MyNode *connfdHead;
    struct MyNode *connfdTail;
    connfdHead = (struct MyNode *)malloc(sizeof(struct MyNode));
    connfdTail = connfdHead;
    int listenRange;
    listenRange = my_max_int(listenfd, listenRange);

    printf("listenfd is:[%d]\n", listenfd);
    while (1) {
        ready_set = read_set;
        select(listenRange + 1, &ready_set, NULL, NULL, NULL);
        if (FD_ISSET(STDIN_FILENO, &ready_set)) {
            command();
        } else if (FD_ISSET(listenfd, &ready_set)) {
            clientlen = sizeof(struct sockaddr_storage);
            connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
            getnameinfo((struct sockaddr *)&clientaddr, clientlen, client_hostname, MYNAME_SIZE, client_port, MYNAME_SIZE, 0);
            printf("accept client:[%s:%s];connfd is:[%d]\n", client_hostname, client_port, connfd);
            // 为了实现同时监听多个fd，将连接fd放入监听集合
            FD_SET(connfd, &read_set);
            struct MyNode *tmp;
            tmp = (struct MyNode *)malloc(sizeof(struct MyNode));
            tmp->val = connfd;
            tmp->pre = connfdTail;
            connfdTail->next = tmp;
            connfdTail = tmp;
            listenRange = my_max_int(connfd, listenRange);
        } else {
            // 遍历connfd
            struct MyNode *tmp = connfdHead;
            while (tmp->next != NULL) {
                if (FD_ISSET(tmp->next->val, &ready_set)) {
                    mySay(tmp->next->val);
                }
                tmp = tmp->next;
            }
            // 如果连接fd退出，需要将连接fd从read_set移出
        }
    }
}

/**
 * 只能服务一个client
 */
void mylisten() {
    int listenfd, connfd;
    struct sockaddr_storage clientaddr;
    socklen_t clientlen;
    char client_hostname[MYNAME_SIZE], client_port[MYNAME_SIZE];
    // 监听，并返回fd
    listenfd = open_listenfd("9090");
    printf("listenfd is:[%d]\n", listenfd);
    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
        getnameinfo((struct sockaddr *)&clientaddr, clientlen, client_hostname, MYNAME_SIZE, client_port, MYNAME_SIZE, 0);
        printf("accept client:[%s:%s]\n", client_hostname, client_port);
        while (1) {
            mySay(connfd);
        }
    }
}

void mySay(int fd) {
    char net_buf[MYMSG_SIZE];
    int nums;
    memset(net_buf, 0, MYMSG_SIZE);
    while ((nums = read(fd, net_buf, MYMSG_SIZE)) > 0) {
        printf("read msg:[%s],read bytes:[%d],strlen:[%d]\n", net_buf, nums, strlen(net_buf));
        nums = write(fd, net_buf, strlen(net_buf));
        printf("write msg:[%s],write bytes:[%d],strlen:[%d]\n", net_buf, nums, strlen(net_buf));
        if (strlen(net_buf) < MYMSG_SIZE) {
            break;
        }
        memset(net_buf, 0, MYMSG_SIZE);
    }
}

void mytest() {
    int listenfd;
    listenfd = open_listenfd("9090");
    printf("listenfd is:[%d]\n", listenfd);
}

void onMyInt(int signo) {
    printf("receve signal[%d]\n", signo);
    exit(0);
}

void command() {
    char cmd[MYCMD_SIZE];
    if (fgets(cmd, MYCMD_SIZE, stdin) < 0) {
        exit(0);
    }
    printf("cmd is:[%s]\n", cmd);
}