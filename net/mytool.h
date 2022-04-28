#ifndef MYTOOL_H
#define MYTOOL_H
#endif

#define MYMSG_SIZE 512
#define MYNAME_SIZE 128
#define MYMSG_BUFF_SIZE 512
#define MYCMD_SIZE 512

int open_clientfd(char *hostname, char *port);

int open_listenfd(char *port);

// 自定义readall方法
int my_readall(int fd, char *buf, int size);

// 自定义双向链表listNode
struct MyNode {
    int val;
    struct MyNode *pre;
    struct MyNode *next;
};

// max
int my_max_int(int a, int b);