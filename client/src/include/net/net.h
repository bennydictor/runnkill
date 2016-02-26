#ifndef NET_CLIENT_H
#define NET_CLIENT_H

#ifdef _WIN32
	#pragma comment ( lib, "ws2_32.lib" )
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
	#include <winsock2.h>
	#include <windows.h>
    #define SYSTEM_PAUSE system("pause")
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <errno.h>
	#include <unistd.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>

	#define WSAGetLastError() errno 
	#define closesocket(X) close(X)
	#define SOCKET_ERROR -1
	#define WSACleanup() ;
	#define HOSTENT hostent
	#define SYSTEM_PAUSE system("wait")

	typedef	int SOCKET;
#endif


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
