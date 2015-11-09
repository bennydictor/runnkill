#ifndef NET_CLIENT_H
#define NET_CLIENT_H

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <stdio.h>


typedef struct {
    pthread_t thread;
    struct sockaddr_in addr;
    socklen_t addr_len;
    int ctrl_sock_fd;
    int data_sock_fd;
    char alive;
} client_t;


extern int control_socket;
extern int data_socket;

int init_net(void);
void free_net(void);
int spawn_client_handler(void);

#endif // NET_CLIENT_H
