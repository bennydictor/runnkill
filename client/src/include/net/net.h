#ifndef NET_CLIENT_H
#define NET_CLIENT_H

#include <netinet/in.h>
#include <netinet/tcp.h>

#include <graphics/draw_obj.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int local_socket;

int init_net(const char *hostname, uint16_t port);
int net_update(char *evs, int *draw_obj_count, draw_obj *draw_objs);
void free_net(void);

#ifdef __cplusplus
}
#endif

#endif // NET_CLIENT_H
