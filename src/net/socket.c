#include <net/socket.h>
#include <util/log.h>

#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

#define BACKLOG 64

int make_local_socket(uint16_t port) {
    int ret = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ret == -1) {
        printl(LOG_W, "Error while making local socket: cannot make socket (%s)", strerror(errno));
        return -1;
    }
    if (fcntl(ret, F_SETFL, O_NONBLOCK) == -1) {
        printl(LOG_W, "Error while making local socket: cannot make socket non-bloking (%s)", strerror(errno));
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(ret, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) == -1) {
        printl(LOG_W, "Error while making local socket: cannot bind socket to localhost (%s)", strerror(errno));
        return -1;
    }
    return ret;
}
