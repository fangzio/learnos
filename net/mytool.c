#include "mytool.h"
#include "netdb.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/socket.h"
#include "unistd.h"

// 队列内请求数量达到最大值，再来的请求，拒绝服务
#define MAX_REQUEST 128

/**
 * 打开客户fd
 */
int open_clientfd(char *hostname, char *port) {
    struct addrinfo *tai, req, *rlist;
    struct sockaddr_in *sai;
    int clientfd;
    memset(&req, 0, sizeof(struct addrinfo));
    req.ai_family = AF_INET;
    req.ai_socktype = SOCK_STREAM;
    req.ai_flags = AI_NUMERICSERV | AI_ADDRCONFIG;
    // 获取socket addr结构
    int rc = getaddrinfo(hostname, port, &req, &rlist);
    if (rc != 0) {
        perror("getaddrinfo err");
        printf("errmsg:%s\n", gai_strerror(rc));
        exit(0);
    }
    for (tai = rlist; tai; tai = rlist->ai_next) {
        // sai = (struct sockaddr_in *)(tai->ai_addr);
        // 获取socket描述符
        clientfd = socket(tai->ai_family, tai->ai_socktype, tai->ai_protocol);
        if (clientfd == -1) {
            perror("socket get clientfd err");
            printf("errmsg:%s\n", gai_strerror(rc));
            continue;
        }
        // 连接服务器
        rc = connect(clientfd, tai->ai_addr, sizeof(struct addrinfo));
        if (rc == 0) {
            break;
        } else {
            // 连接失败，关闭fd
            close(clientfd);
            perror("connect err");
            printf("errmsg:%s\n", gai_strerror(rc));
            exit(0);
        }
    }
    freeaddrinfo(rlist);
    return clientfd;
}

/**
 * 打开监听fd
 */
int open_listenfd(char *port) {
    struct addrinfo req, *tai, *rlist;
    int rc, listenfd;
    int optval = 1;
    memset(&req, 0, sizeof(struct addrinfo));
    req.ai_family = AF_INET;
    req.ai_socktype = SOCK_STREAM;
    req.ai_flags = AI_PASSIVE | AI_NUMERICSERV | AI_ADDRCONFIG;
    rc = getaddrinfo(NULL, port, &req, &rlist);
    if (rc != 0) {
        perror("getaddrinfo err");
        printf("errmsg:%s\n", gai_strerror(rc));
        exit(0);
    }
    for (tai = rlist; tai; tai = rlist->ai_next) {
        // 获取socket描述符
        if ((listenfd = socket(tai->ai_family, tai->ai_socktype, tai->ai_protocol)) < 0) {
            perror("socket err");
            continue;
        }
        // 消除绑定时提示地址已被占用的错误
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int));
        // 绑定描述符和地址
        rc = bind(listenfd, tai->ai_addr, tai->ai_addrlen);
        if (rc != 0) {
            close(listenfd);
            perror("bind err");
            printf("errmsg:%s\n", gai_strerror(rc));
            continue;
        }
        break;
    }
    if (!tai) {
        return -1;
    }
    // clean
    freeaddrinfo(rlist);
    // 监听描述符
    rc = listen(listenfd, MAX_REQUEST);
    if (rc != 0) {
        close(listenfd);
        perror("listen err");
        printf("errmsg:%s\n", gai_strerror(rc));
    }
    return listenfd;
}

int my_readall(int fd, char *buf, int size) {
    int nums, n;
    char all[size], tmp[MYMSG_BUFF_SIZE];
    memset(all, 0, size);
    memset(tmp, 0, MYMSG_BUFF_SIZE);
    while ((nums = read(fd, tmp, MYMSG_BUFF_SIZE)) > 0) {
        strcat(all, tmp);
        // 拼接完，还给追加了一个空格，去掉
        if (nums == MYMSG_BUFF_SIZE) {
            all[strlen(all) - 1] = '\0';
        }
        if (nums < MYMSG_BUFF_SIZE) {
            break;
        }
        memset(tmp, 0, MYMSG_BUFF_SIZE);
    }
    strcpy(buf, all);
    return nums;
}

int my_max_int(int a, int b) {
    if (a >= b) {
        return a;
    }
    return b;
}