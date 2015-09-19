#include <util/timing.h>
#include <graphics/glfw.h>


float prev_time;
float last_point;
unsigned int fps, cur_fps;

float delta(void) {
    ++cur_fps;
    float cur_time = glfwGetTime();
    if (cur_time - last_point >= 1) {
        printl(LOG_D, "fps: %d\n", fps = cur_fps);
        cur_fps = 0;
        last_point = cur_time;
    }
    float d = cur_time - prev_time;
    prev_time = cur_time;
    return d;
}
