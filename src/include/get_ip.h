#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string>

#include "glog/logging.h"

using std::string;

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

string GetIP() {
    char ip[16] = {0};
    get_ip(ip);
    return ip;
}

