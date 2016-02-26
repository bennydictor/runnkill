#include <net/socket.h>
#include <util/log.h>

#ifdef _WIN32
	#pragma comment ( lib, "ws2_32.lib" )
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
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

#include <string.h>

int make_local_udp_socket(void) {
    SOCKET ret = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ret == -1) {
        printl(LOG_W, "Error while making local udp socket: cannot make socket (%s)", strerror(errno));
        return -1;
    }
    return ret;
}

int make_local_tcp_socket(void) {
    SOCKET ret = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ret == -1) {
        printl(LOG_W, "Error while making local tcp socket: cannot make socket (%s)", strerror(errno));
        return -1;
    }
    return ret;
}
