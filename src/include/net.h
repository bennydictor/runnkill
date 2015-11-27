#ifndef NET_CLIENT_H
#define NET_CLIENT_H

#include <game/world.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int local_socket;

int init_net(const char *hostname, uint16_t port);
void free_net(void);
void net_update(void);

#ifdef __cplusplus
}
#endif

#endif // NET_CLIENT_H
