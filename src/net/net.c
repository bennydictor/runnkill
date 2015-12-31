#include <net/net.h>
#include <net/proto.h>
#include <netdb.h>
#include <poll.h>
#include <util/log.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include <world.h>
#include <graphics/gl.h>
#include <graphics/objects/sphere.h>
#include <graphics/objects/sphere_sector.h>
#include <graphics/objects/field.h>

#define MSG_BUF_LEN (1024 * 1024 * 1024)
#define PORT 12345
#define GET(TYPE, VAL) do { VAL = *(TYPE *) ptr; ptr += sizeof(TYPE); } while(0)


char msg[MSG_BUF_LEN];
int local_socket;
struct sockaddr_in server;
socklen_t server_addlen;
int client_num;

ssize_t recvfrom_timeout(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen) {
    struct pollfd fd;
    fd.fd = sockfd;
    fd.events = POLLIN;
    if (poll(&fd, 1, 1000) <= 0) {
        printl(LOG_W, "Error while receiving datagram: Timed out");
        return -1;
    }
    return recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

int init_net(const char *hostname, uint16_t port) {
    local_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct hostent *host = gethostbyname(hostname);
    if (!host) {
        printl(LOG_W, "Error while initializing network: cannot get %s's ip address", hostname);
        return 1;
    }
    server_addlen = sizeof(struct sockaddr_in);
    server.sin_family = AF_INET;
    server.sin_port = htons(port == 0 ? PORT : port);
    server.sin_addr.s_addr = *(in_addr_t *) host->h_addr_list[0];
    msg[0] = MSG_HELLO;
    sendto(local_socket, msg, 1, 0, (struct sockaddr *) &server, server_addlen); 
    if (recvfrom_timeout(local_socket, msg, MSG_BUF_LEN, 0, NULL, NULL) == -1 || msg[0] == MSG_NOPE) {
        printl(LOG_W, "Error while initializing network: cannot login to %s", host->h_name);
        return 1;
    }
    client_num = msg[1];
    printl(LOG_I, "Logged in as client %d", client_num + 1);

    msg[0] = MSG_RES;
    msg[1] = client_num;
    msg[2] = RES_MAP;
    sendto(local_socket, msg, 3, 0, (struct sockaddr *) &server, server_addlen); 
    if (recvfrom_timeout(local_socket, msg, MSG_BUF_LEN, 0, NULL, NULL) == -1 || msg[0] == MSG_NOPE) {
        printl(LOG_W, "Error while initializing network: cannot get map");
        return 1;
    }
    void *ptr = msg + 1;
    GET(int, world_w);
    GET(int, world_h);
    world_field = malloc(world_w * sizeof(int *));
    for (int i = 0; i < world_w; ++i) {
        world_field[i] = malloc(world_h * sizeof(int));
        for (int j = 0; j < world_h; ++j) {
            GET(int, world_field[i][j]);
        }
    }
    printl(LOG_D, "Got map!");

    msg[0] = MSG_RES;
    msg[1] = client_num;
    msg[2] = RES_LIGHT;
    sendto(local_socket, msg, 3, 0, (struct sockaddr *) &server, server_addlen); 
    if (recvfrom_timeout(local_socket, msg, MSG_BUF_LEN, 0, NULL, NULL) == -1 || msg[0] == MSG_NOPE) {
        printl(LOG_W, "Error while initializing network: cannot get lights");
        return 1;
    }
    ptr = msg + 1;
    for (int i = 0; i < LIGHT_COUNT; ++i) {
        GET(char, gl_light_enable[i]);
        if (gl_light_enable[i]) {
            gl_light[i].pos = malloc(3 * sizeof(float));
            gl_light[i].rot = malloc(3 * sizeof(float));
            gl_light[i].ambient = malloc(3 * sizeof(float));
            gl_light[i].diffuse = malloc(3 * sizeof(float));
            gl_light[i].specular = malloc(3 * sizeof(float));
            GET(float, gl_light[i].pos[0]);
            GET(float, gl_light[i].pos[1]);
            GET(float, gl_light[i].pos[2]);
            GET(float, gl_light[i].rot[0]);
            GET(float, gl_light[i].rot[1]);
            GET(float, gl_light[i].rot[2]);
            GET(float, gl_light[i].ambient[0]);
            GET(float, gl_light[i].ambient[1]);
            GET(float, gl_light[i].ambient[2]);
            GET(float, gl_light[i].diffuse[0]);
            GET(float, gl_light[i].diffuse[1]);
            GET(float, gl_light[i].diffuse[2]);
            GET(float, gl_light[i].specular[0]);
            GET(float, gl_light[i].specular[1]);
            GET(float, gl_light[i].specular[2]);
            GET(float, gl_light[i].fov);
            GET(float, gl_light[i].z_near);
            GET(float, gl_light[i].z_far);
        }
    }
    printl(LOG_D, "Got lights!");

    msg[0] = MSG_RES;
    msg[1] = client_num;
    msg[2] = RES_MATERIAL;
    sendto(local_socket, msg, 3, 0, (struct sockaddr *) &server, server_addlen); 
    if (recvfrom_timeout(local_socket, msg, MSG_BUF_LEN, 0, NULL, NULL) == -1 || msg[0] == MSG_NOPE) {
        printl(LOG_W, "Error while initializing network: cannot get materials");
        return 1;
    }
    ptr = msg + 1;
    GET(int, material_count);
    for (int i = 0; i < material_count; ++i) {
        materials[i].ambient = malloc(3 * sizeof(float));
        materials[i].diffuse = malloc(3 * sizeof(float));
        materials[i].specular = malloc(3 * sizeof(float));
        GET(float, materials[i].ambient[0]);
        GET(float, materials[i].ambient[1]);
        GET(float, materials[i].ambient[2]);
        GET(float, materials[i].diffuse[0]);
        GET(float, materials[i].diffuse[1]);
        GET(float, materials[i].diffuse[2]);
        GET(float, materials[i].specular[0]);
        GET(float, materials[i].specular[1]);
        GET(float, materials[i].specular[2]);
        GET(float, materials[i].shininess);
    }
    printl(LOG_D, "Got materials!");

    return 0;
}

int net_update(char *evs, int *draw_obj_count, draw_obj *draw_objs) {
    msg[0] = MSG_UPD;
    msg[1] = client_num;
    memcpy(msg + 2, evs, WORLD_EVENT_COUNT);
    float orient[3] = {sinf(gl_rot[1]), -gl_rot[0] * 2 + 0.1, -cosf(gl_rot[1])};
    memcpy(msg + 2 + WORLD_EVENT_COUNT, orient, 3 * sizeof(float));
    sendto(local_socket, msg, 2 + WORLD_EVENT_COUNT + 3 * sizeof(float), 0, (struct sockaddr *) &server, server_addlen); 
    if (recvfrom_timeout(local_socket, msg, MSG_BUF_LEN, 0, NULL, NULL) == -1 || msg[0] == MSG_NOPE) {
        printl(LOG_W, "Error while updating player: cannot send update data");
        return 1;
    }
    msg[0] = MSG_RES;
    msg[1] = client_num;
    msg[2] = RES_DRAW;
    sendto(local_socket, msg, 3, 0, (struct sockaddr *) &server, server_addlen);
    if (recvfrom_timeout(local_socket, msg, MSG_BUF_LEN, 0, NULL, NULL) == -1 || msg[0] == MSG_NOPE) {
        printl(LOG_W, "Error while updating player: cannot get draw objects");
        return 1;
    }
    void *ptr = msg + 1;
    char type;
    float pos[3];
    float rad, rot;
    int s;
    short int mat_id;
    GET(float, coords[0]);
    GET(float, coords[1]);
    GET(float, coords[2]);
    GET(float, speed[0]);
    GET(float, speed[1]);
    GET(float, speed[2]);
    GET(float, orientation[0]);
    GET(float, orientation[1]);
    GET(float, orientation[2]);
    GET(float, hp);
    GET(float, mp);
    GET(float, max_hp);
    GET(float, max_mp);
    GET(float, agility);
    GET(float, strength);
    GET(float, intellect);
    GET(float, abs_speed);
    GET(float, jump_high);
    GET(int, *draw_obj_count);
    gl_pos[0] = coords[0] - 5 * orientation[0];
    gl_pos[1] = coords[1] + 1;
    gl_pos[2] = coords[2] - 5 * orientation[2];
    ++*draw_obj_count;
    draw_objs[0] = make_draw_field(materials[0]);
    for (int i = 1; i < *draw_obj_count; ++i) {
        GET(char, type);
        GET(float, pos[0]);
        GET(float, pos[1]);
        GET(float, pos[2]);
        GET(float, rad);
        GET(float, rot);
        GET(int, s);
        GET(short int, mat_id);
        if (type == DRAW_SPHERE) {
            draw_objs[i] = make_draw_sphere3fv1f(pos, rad, materials[mat_id]);
        } else if (type == DRAW_SPHERE_SECTOR) {
            draw_objs[i] = make_draw_sphere_sector3fv2f(pos, rot, rad, s, materials[mat_id]);
        }
    }
    return 0;
}

void free_net(void) {
    msg[0] = MSG_EXIT;
    msg[1] = client_num;
    sendto(local_socket, msg, 2, 0, (struct sockaddr *) &server, server_addlen);
    recvfrom_timeout(local_socket, msg, 1, 0, NULL, NULL);
    if (msg[0] == MSG_OK) {
        printl(LOG_I, "Logout sucessful");
    } else {
        printl(LOG_I, "Logout failed");
    }
    close(local_socket);
}
