#include <bullet.h>

using namespace std;

vec2<float> bullet::in_time(float time) {
    return coords + time * speed;
}
