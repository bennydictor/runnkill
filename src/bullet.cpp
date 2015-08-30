#include <bullet.h>

using namespace std;

vec3<float> bullet::in_time(float time) {
    return coords + time * speed;
}
