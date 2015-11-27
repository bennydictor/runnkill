#include <net.h>
#include <net/proto.h>
#include <netdb.h>
#include <util/log.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>


#define MSG_BUF_LEN 128
#define PORT 12345

char msg[MSG_BUF_LEN];
int local_socket;
struct sockaddr_in server;
socklen_t addrlen;
int client_num;

int init_net(const char *hostname, uint16_t port) {
    local_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct hostent *host = gethostbyname(hostname);
    if (!host) {
        printl(LOG_W, "Error while initializing network: cannot get %s's ip address", hostname);
        return 1;
    }
    addrlen = sizeof(struct sockaddr_in);
    server.sin_family = AF_INET;
    server.sin_port = htons(port == 0 ? PORT : port);
    server.sin_addr.s_addr = *(in_addr_t *) host->h_addr_list[0];
    msg[0] = MSG_HELLO;
    sendto(local_socket, msg, 1, 0, (struct sockaddr *) &server, addrlen); 
    recvfrom(local_socket, msg, MSG_BUF_LEN, 0, NULL, NULL);
    if (msg[0] == MSG_NOPE) {
        printl(LOG_W, "Error while initializing network: cannot login to %s", host->h_name);
        return 1;
    }
    client_num = msg[1];
    printl(LOG_I, "Logged in as client %d", client_num + 1);
    return 0;
}

void free_net(void) {
    msg[0] = MSG_EXIT;
    msg[1] = client_num;
    sendto(local_socket, msg, 2, 0, (struct sockaddr *) &server, addrlen);
    recvfrom(local_socket, msg, 1, 0, NULL, NULL);
    if (msg[0] == MSG_OK) {
        printl(LOG_I, "Logout sucessful");
    } else {
        printl(LOG_I, "Logout failed");
    }
    close(local_socket);
}
