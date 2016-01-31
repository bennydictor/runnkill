#ifndef GAME_MESSAGE_H
#define GAME_MESSAGE_H

#include <cstring>

struct message {
    float time;
    char* str;
    int l;
    message() {
        l = -1;
    }
    int len() {
        if (l == -1) {
            l = strlen(str);
        }
        return l;
    }
};

#endif //GAME_MESSAGE_H
