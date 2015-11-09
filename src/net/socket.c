#include <net/socket.h>
#include <util/log.h>

#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>

#define BACKLOG 64

int make_local_tcp_socket(uint16_t port) {
    int ret = socket(AF_INET, SOCK_STREAM, 0);
    if (ret == -1) {
        printl(LOG_W, "Error while making local tcp socket: cannot make socket");
        return -1;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = 0;
    if (bind(ret, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) == -1) {
        printl(LOG_W, "Error while making local tcp socket: cannot bind socket to localhost (%s)", strerror(errno));
        return -1;
    }
    if (listen(ret, BACKLOG) == -1) {
        printl(LOG_W, "Error while making local tcp socket: cannot open socket (%s)", strerror(errno));
        return -1;
    }
    return ret;
}

int make_local_udp_socket(uint16_t port) {
    int ret = socket(AF_INET, SOCK_DGRAM, 0);
    if (ret == -1) {
        printl(LOG_W, "Error while making local udp socket: cannot make socket (%s)", strerror(errno));
        return -1;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = 0;
    if (bind(ret, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) == -1) {
        printl(LOG_W, "Error while making local udp socket: cannot bind socket to localhost (%s)", strerror(errno));
        return -1;
    }
    return ret;
}
