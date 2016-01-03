#include <net/socket.h>
#include <util/log.h>

#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

int make_local_udp_socket(void) {
    int ret = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ret == -1) {
        printl(LOG_W, "Error while making local udp socket: cannot make socket (%s)", strerror(errno));
        return -1;
    }
    return ret;
}

int make_local_tcp_socket(void) {
    int ret = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ret == -1) {
        printl(LOG_W, "Error while making local tcp socket: cannot make socket (%s)", strerror(errno));
        return -1;
    }
    return ret;
}
