#include "arpa/inet.h"
#include "netdb.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/socket.h"

#define HOST_LEN 100

void main(int argc, char **argv) {
    if (argc != 2) {
        printf("please input host\n");
        exit(0);
    }
    int rc, flags;
    struct addrinfo *p, *listp, hints;
    char buf[HOST_LEN];
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (rc = getaddrinfo(argv[1], NULL, &hints, &listp) != 0) {
        perror("getaddrinfo err");
        const char *err = gai_strerror(rc);
        printf("errmsg is:[%s]\n", err);
        exit(0);
    }
    flags = NI_NUMERICHOST;
    for (p = listp; p; p = p->ai_next) {
        struct sockaddr_in *tmp = (struct sockaddr_in *)p->ai_addr;
        printf("%u\n", tmp->sin_addr.s_addr);
        // getnameinfo(p->ai_addr, p->ai_addrlen, buf, HOST_LEN, NULL, 0,
        // flags); printf("host is:[%s]\n", buf);
        const char *rs = inet_ntop(AF_INET, &tmp->sin_addr.s_addr, buf, HOST_LEN);
        printf("ip is:[%s]\n", buf);
    }
    freeaddrinfo(listp);
}