#include <net/client.h>
#include <net/socket.h>
#include <util/log.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <assert.h>

#define PORT 12345
#define MAX_CLIENTS 256


int control_socket;
int data_socket;
int client_count;
client_t clients[MAX_CLIENTS];

pthread_attr_t thread_attr;


void *client_handler(void *arg) {
    client_t *client = arg;
    int client_num = client - clients;
    printl(LOG_I, "Client %d is online", client_num + 1);
    char buf[] = "Hello!\nFor now, we can only send your messages back to you.\nC-c to exit.\n";
    int buflen = strlen(buf);
    assert(write(client->ctrl_sock_fd, buf, buflen) <= buflen);
    int curlen;
    while ((curlen = read(client->ctrl_sock_fd, buf, buflen)) > 0) {
        assert(write(client->ctrl_sock_fd, buf, curlen) <= curlen);
    }
    client->alive = 0;
    close(client->ctrl_sock_fd);
    printl(LOG_I, "Client %d is offline", client_num + 1);
    return arg;
}

void *client_accepter(void *arg) {
    for (;;) {
        if (client_count == MAX_CLIENTS) {
            printl(LOG_W, "Error while accepting client: no free client slots avalible");
            return arg;
        }
        clients[client_count].addr_len = sizeof(struct sockaddr_in);
        clients[client_count].ctrl_sock_fd = accept(control_socket, (struct sockaddr *) &clients[client_count].addr, &clients[client_count].addr_len);
        if (clients[client_count].ctrl_sock_fd == -1) {
            printl(LOG_W, "Error while accepting client: cannot accept connection");
            continue;
        }
        clients[client_count].alive = 1;
        if (pthread_create(&clients[client_count].thread, &thread_attr, client_handler, clients + client_count) != 0) {
            printl(LOG_W, "Error while spawing client handler");
            clients[client_count].alive = 0;
            close(clients[client_count].ctrl_sock_fd);
            continue;
        }
        ++client_count;
    }
}

int init_net(void) {
    if (pthread_attr_init(&thread_attr) != 0) {
        printl(LOG_W, "Error while initializing network: cannot initialize thread attributes");
        return 1;
    }
    control_socket = make_local_tcp_socket(PORT);
    if (control_socket == -1) {
        printl(LOG_W, "Error while initializing network: cannot make control socket");
        pthread_attr_destroy(&thread_attr);;
        return 1;
    }
    data_socket = make_local_udp_socket(PORT);
    if (data_socket == -1) {
        printl(LOG_W, "Error while initializing network: cannot make data socket");
        shutdown(control_socket, SHUT_RDWR);
        pthread_attr_destroy(&thread_attr);;
        return 1;
    }
    if (pthread_create(&clients[client_count].thread, &thread_attr, client_accepter, NULL) != 0) {
        printl(LOG_W, "Error while spawing client accepter");
        return 1;
    }
    return 0;
}

void free_net(void) {
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i].alive) {

        }
    }
    close(control_socket);
    close(data_socket);
    pthread_attr_destroy(&thread_attr);
}
