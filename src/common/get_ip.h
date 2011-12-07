#ifndef SRC_INCLUDE_GET_IP_H
#define SRC_INCLUDE_GET_IP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>
#include <netdb.h>
#include <string>
#include <errno.h>

#include <glog/logging.h>

#define ETH_NAME    "eth0"

using std::string;
#if 0
int get_ip(char* str) {
    addrinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_flags = AI_CANONNAME;
    hint.ai_socktype = SOCK_STREAM;
    char hostname[100] =  {0};
    gethostname(hostname, sizeof(hostname));
    int ret;
    addrinfo* res;
    if ((ret = getaddrinfo(hostname, NULL, &hint, &res)) < 0) {
        LOG(ERROR) << "get ip error: " << gai_strerror(ret);
        return ret;
    }
    addrinfo* curr = res;
    if (curr != NULL) {
        // get the fisrt address as ip
        char* ip = inet_ntoa(((sockaddr_in* )(res->ai_addr))->sin_addr);
        strcpy(str, ip);
        while (curr != NULL) {
            addrinfo* tmp = curr->ai_next;
            freeaddrinfo(curr);
            curr = tmp;
        }
        return 0;
    } else {
        return -1;
    }
}
#endif
int get_ip(char * ip)
{   
    int sock;
    struct sockaddr_in sin;
    struct ifreq ifr;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        return -1;      
    }
    strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ - 1] = 0;
    if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
        perror("ioctl");
        return -1;
    }
    memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
    char * tmp = inet_ntoa(sin.sin_addr);
    strncpy(ip, tmp, strlen(tmp));
    return 0;
}

string GetIP() {
    char ip[16] = {0};
    get_ip(ip);
    return ip;
}

#endif
