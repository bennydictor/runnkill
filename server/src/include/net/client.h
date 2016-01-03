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
    char evs[WORLD_EVENT_COUNT];
    float orientation[3];
    int tcp;
    char alive;
} client_t;

#define MAX_CLIENTS 256

extern int local_socket;
extern int client_count;
extern client_t clients[MAX_CLIENTS];

int init_net(void);
void free_net(void);
void net_update(void);

#ifdef __cplusplus
}
#endif

#endif // NET_CLIENT_H
