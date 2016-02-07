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
#include <game/material.h>
#include <game/init_world.h>

#define MAX_CLIENTS 256
#define PORT 12345
#define PORT_STR #PORT

#define MSG_BUF_LEN (1024 * 1024 * 1024)
#define PUT(TYPE, VAL) do { *(TYPE *) ptr = (VAL); ptr += sizeof(TYPE); } while(0)

int local_tcp_socket, local_udp_socket;
int client_count;
client_t clients[MAX_CLIENTS];
char msg[MSG_BUF_LEN];

void net_update(void) {
    struct sockaddr_in src_addr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    memset(&src_addr, 0, sizeof(struct sockaddr_in));
    int msg_len, client_sock;
    while ((msg_len = recvfrom(local_udp_socket, msg, MSG_BUF_LEN, MSG_DONTWAIT, (struct sockaddr *) &src_addr, &addrlen)) > 0) {
        switch (msg[0]) {
        case MSG_HELLO:
            if (msg_len == 1 && client_count < MAX_CLIENTS) {
                clients[client_count].tcp = accept(local_tcp_socket, NULL, NULL);
                if (clients[client_count].tcp == -1) {
                    printl(LOG_W, "Error while accepting new client: cannot accept tcp connection");
                    msg[0] = MSG_NOPE;
                    sendto(local_udp_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
                    break;
                }
                clients[client_count].alive = 1;
                add_player("Derrior", 1);
                printl(LOG_I, "Client %d is online", client_count + 1);
                msg[0] = MSG_OK;
                msg[1] = client_count;
                sendto(local_udp_socket, msg, 2, 0, (struct sockaddr *) &src_addr, addrlen);
                ++client_count;
            } else {
                msg[0] = MSG_NOPE;
                sendto(local_udp_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
            }
            break;
        case MSG_PING:
            msg[0] = MSG_PONG;
            sendto(local_udp_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
            break;
        case MSG_EXIT:
            if (msg_len == 2 && clients[(int) msg[1]].alive) {
                clients[(int) msg[1]].alive = 0;
                kill_person((int) msg[1]);
                close(clients[(int) msg[1]].tcp);
                printl(LOG_I, "Client %d is offline", msg[1] + 1);
                msg[0] = MSG_OK;
                sendto(local_udp_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
            } else {
                msg[0] = MSG_NOPE;
                sendto(local_udp_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
            }
            break;
        case MSG_RES:
            if (msg_len == 3 && clients[(int) msg[1]].alive) {
                client_sock = clients[(int) msg[1]].tcp;
                void *ptr = msg;
                if (msg[2] == RES_MAP) {
                    PUT(char, MSG_OK);
                    PUT(int, world_w);
                    PUT(int, world_h);
                    for (int i = 0; i < world_w; ++i) {
                        for (int j = 0; j < world_h; ++j) {
                            PUT(int, world_field[i][j]);
                        }
                    }
                    sendto(local_udp_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
                    if (msg[0] == MSG_OK) {
                        if (write(client_sock, msg + 1, ((char *) ptr) - msg - 1) != ((char*) ptr) - msg - 1) {
                            printl(LOG_W, "Error while sending resourses: cannot write to socket");
                        }
                    }
                } else if (msg[2] == RES_LIGHT) {
                    PUT(char, MSG_OK);
                    for (int i = 0; i < LIGHT_COUNT; ++i) {
                        PUT(char, gl_light_enable[i]);
                        if (gl_light_enable[i]) {
                            PUT(float, gl_light[i].pos[0]);
                            PUT(float, gl_light[i].pos[1]);
                            PUT(float, gl_light[i].pos[2]);
                            PUT(float, gl_light[i].rot[0]);
                            PUT(float, gl_light[i].rot[1]);
                            PUT(float, gl_light[i].rot[2]);
                            PUT(float, gl_light[i].ambient[0]);
                            PUT(float, gl_light[i].ambient[1]);
                            PUT(float, gl_light[i].ambient[2]);
                            PUT(float, gl_light[i].diffuse[0]);
                            PUT(float, gl_light[i].diffuse[1]);
                            PUT(float, gl_light[i].diffuse[2]);
                            PUT(float, gl_light[i].specular[0]);
                            PUT(float, gl_light[i].specular[1]);
                            PUT(float, gl_light[i].specular[2]);
                            PUT(float, gl_light[i].fov);
                            PUT(float, gl_light[i].z_near);
                            PUT(float, gl_light[i].z_far);
                        }
                    }
                    sendto(local_udp_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
                    if (msg[0] == MSG_OK) {
                        if (write(client_sock, msg + 1, ((char *) ptr) - msg - 1) != ((char*) ptr) - msg - 1) {
                            printl(LOG_W, "Error while sending resourses: cannot write to socket");
                        }
                    }
                } else if (msg[2] == RES_MATERIAL) {
                    PUT(char, MSG_OK);
                    PUT(int, material_count);
                    for (int i = 0; i < material_count; ++i) {
                        PUT(float, materials[i].ambient[0]);
                        PUT(float, materials[i].ambient[1]);
                        PUT(float, materials[i].ambient[2]);
                        PUT(float, materials[i].diffuse[0]);
                        PUT(float, materials[i].diffuse[1]);
                        PUT(float, materials[i].diffuse[2]);
                        PUT(float, materials[i].specular[0]);
                        PUT(float, materials[i].specular[1]);
                        PUT(float, materials[i].specular[2]);
                        PUT(float, materials[i].shininess);
                        PUT(float, materials[i].alpha);
                    }
                    sendto(local_udp_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
                    if (msg[0] == MSG_OK) {
                        if (write(client_sock, msg + 1, ((char *) ptr) - msg - 1) != ((char*) ptr) - msg - 1) {
                            printl(LOG_W, "Error while sending resourses: cannot write to socket");
                        }
                    }
                } else if (msg[2] == RES_DRAW) {
                    PUT(char, MSG_OK);
                    int client = msg[1];
                    memcpy(ptr, get_person_data_begin(client), get_person_data_end(client) - get_person_data_begin(client));
                    ptr += get_person_data_end(client) - get_person_data_begin(client);
                    PUT(float, get_person_business(client));
                    PUT(float, get_person_max_business(client));
                    PUT(int, draw_obj_count);
                    for (int i = 0; i < draw_obj_count; ++i) {
                        PUT(char, draw_objs[i].type);
                        PUT(float, draw_objs[i].pos[0]);
                        PUT(float, draw_objs[i].pos[1]);
                        PUT(float, draw_objs[i].pos[2]);
                        PUT(float, draw_objs[i].rad);
                        PUT(float, draw_objs[i].rot);
                        PUT(int, draw_objs[i].s);
                        PUT(short int, draw_objs[i].material);
                    }
                    /* 
                    char* messages = get_person_text(client);
                    memcpy(ptr, messages, strlen(messages));
                    */
                    sendto(local_udp_socket, msg, ((char *) ptr) - msg, 0, (struct sockaddr *) &src_addr, addrlen);
                    
                } else {
                    PUT(char, MSG_NOPE);
                    sendto(local_udp_socket, msg, ((char *) ptr) - msg, 0, (struct sockaddr *) &src_addr, addrlen);
                }
            } else {
                msg[0] = MSG_NOPE;
                sendto(local_udp_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
            }
            break;
        case MSG_UPD:
            if (msg_len == 2 + WORLD_EVENT_COUNT + 3 * sizeof(float) && clients[(int) msg[1]].alive) {
                memcpy(clients[(int) msg[1]].evs, msg + 2, WORLD_EVENT_COUNT);
                memcpy(clients[(int) msg[1]].orientation, msg + 2 + WORLD_EVENT_COUNT, 3 * sizeof(float));
                msg[0] = MSG_OK;
                sendto(local_udp_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
            } else {
                msg[0] = MSG_NOPE;
                sendto(local_udp_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
            }
            break;
        default:
            msg[0] = MSG_NOPE;
            sendto(local_udp_socket, msg, 1, 0, (struct sockaddr *) &src_addr, addrlen);
            printl(LOG_W, "Error while receiving message: wrong message type");
            break;
        }
    }
}

int init_net(void) {
    local_udp_socket = make_local_udp_socket(PORT);
    if (local_udp_socket == -1) {
        printl(LOG_W, "Error while initializing network: cannot make local udp socket");
        return 1;
    }
    local_tcp_socket = make_local_tcp_socket(PORT);
    if (local_tcp_socket == -1) {
        printl(LOG_W, "Error while initializing network: cannot make local tcp socket");
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
    close(local_udp_socket);
}
