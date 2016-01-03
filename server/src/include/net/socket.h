#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include <stdint.h>


int make_local_tcp_socket(uint16_t port);
int make_local_udp_socket(uint16_t port);

#endif // NET_SOCKET_H
