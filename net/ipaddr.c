#include "arpa/inet.h"
#include "stdio.h"

void ntop();
void pton();

void main() { pton(); }

void ntop() {
    uint32_t binaddr;
    // 0x8002c2f2
    printf("please input addr\n");
    scanf("%x", &binaddr);
    binaddr = htonl(binaddr);
    char dst[INET_ADDRSTRLEN];
    const char *rs = inet_ntop(AF_INET, &binaddr, dst, sizeof(dst));
    if (rs == NULL) {
        perror("ntop err");
    } else {
        printf("%s\n", rs);
    }
}

void pton() {
    const char ipaddr[INET_ADDRSTRLEN];
    uint32_t ipnum;
    // 128.2.194.242
    printf("please input ip addr:\t");
    scanf("%s", ipaddr);
    int rs = inet_pton(AF_INET, ipaddr, &ipnum);
    if (rs != 1) {
        perror("pton err");
    } else {
        ipnum = htonl(ipnum);
        printf("ipnum is: 0x%x\n", ipnum);
    }
}