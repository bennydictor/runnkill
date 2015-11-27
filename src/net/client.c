#include <net/client.h>
#include <net/socket.h>
#include <net/proto.h>
#include <util/log.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <game/world.h>

#define MAX_CLIENTS 256
#define PORT 12345


int local_socket;
int client_count;
client_t clients[MAX_CLIENTS];

#define MSG_BUF_LEN (128 + world_w * world_h)

void net_update(void) {
    char msg[MSG_BUF_LEN];
    struct sockaddr_in src_addr;
    socklen_t addrlen;
    int msg_len;
    while ((msg_len = recvfrom(local_socket, msg, MSG_BUF_LEN, MSG_DONTWAIT, (struct sockaddr *) &src_addr, &addrlen)) > 0) {
        printl(LOG_D, "Datagramm from %s:%d", inet_ntoa(src_addr.sin_addr), ntohs(src_addr.sin_port));
        switch (msg[0]) {
        case MSG_HELLO:
            if (msg_len == 1 && client_count < MAX_CLIENTS) {
                clients[client_count].addr = src_addr;
                clients[client_count].addrlen = addrlen;
                clients[client_count].alive = 1;
                printl(LOG_I, "Client %d is online", client_count + 1);
                msg[0] = MSG_OK;
                msg[1] = client_count;
                sendto(local_socket, msg, 2, 0, (struct sockaddr *) &src_addr, addrlen);
                ++client_count;
            } else {
                msg[0] = MSG_NOPE;
                sendto(local_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
            }
            break;
        case MSG_PING:
            msg[0] = MSG_PONG;
            sendto(local_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
            break;
        case MSG_EXIT:
            if (msg_len == 2 && clients[(int) msg[1]].alive) {
                clients[(int) msg[1]].alive = 0;
                printl(LOG_I, "Client %d is offline", msg[1] + 1);
                msg[0] = MSG_OK;
                sendto(local_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
            } else {
                msg[0] = MSG_NOPE;
                sendto(local_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
            }
            break;
        case MSG_MAP:
            if (msg_len == 2 && clients[(int) msg[1]].alive) {
                msg[0] = MSG_OK;
                int ptr = 1;
                for (int i = 0; i < world_w; ++i) {
                    for (int j = 0; j < world_h; ++j) {
                        msg[ptr++] = (char) world_field[i][j];
                    }
                }
                sendto(local_socket, msg, 1 + world_w * world_h, 0, (struct sockaddr *) &src_addr, addrlen);
            } else {
                msg[0] = MSG_NOPE;
                sendto(local_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
            }
            break;
        case MSG_UPD:

            break;
        default:
            msg[0] = MSG_NOPE;
            sendto(local_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
            printl(LOG_D, "Wrong messgae type");
            break;
        }
    }
}

int init_net(void) {
    local_socket = make_local_socket(PORT);
    if (local_socket == -1) {
        printl(LOG_W, "Error while initializing network: cannot make local socket");
        return 1;
    }
    return 0;
}

void free_net(void) {
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].alive) {
            clients[i].alive = 0;
            printl(LOG_I, "Client %d is offline", i + 1);
        }
    }
    close(local_socket);
}
