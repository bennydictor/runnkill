#ifndef NET_CLIENT_H
#define NET_CLIENT_H

#include <game/world.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    struct sockaddr_in addr;
    socklen_t addrlen;
    char alive;
} client_t;


extern int local_socket;

int init_net(void);
void free_net(void);
void net_update(void);

#ifdef __cplusplus
}
#endif

#endif // NET_CLIENT_H
