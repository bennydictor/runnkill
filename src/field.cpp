#include <field.h>


using namespace std;

void render_line(vec2<int> p1, vec2<int> p2, bool **buf, bool val) {
    bool steep = abs(p2.y - p1.y) > abs(p2.x - p1.x);
    if (steep) {
        swap(p1.x, p1.y);
        swap(p2.x, p2.y);
    }
    if (p1.x > p2.x) {
        swap(p1.x, p2.x);
        swap(p1.y, p2.y);
    }
    int dx = p2.x - p1.x;
    int dy = abs(p2.y - p1.y);
    int error = dx / 2;
    int ystep = p1.y < p2.y ? 1 : -1;
    int y = p1.y;
    for (int x = p1.x; x <= p2.x; ++x) {
        buf[steep ? y : x][steep ? x : y] = val;
        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
}

int get_rand(int l, int r) {
    return rand() % (r - l) + l; 
}

bool** gen_field(int w, int h) {
    bool **arr = new bool*[w];
    for (int i = 0; i < w; i++) {
        arr[i] = new bool[h];
    }
    int am_of_walls = w;
    int am_of_tunnels = h / 5;
    for (int i = 0; i < am_of_walls + am_of_tunnels; i++) {
        int y_l = get_rand(1, h - 1), y_r = get_rand(1, h - 1);
        int x_l = get_rand(1, w - 1), x_r = get_rand(1, w - 1);
        if (get_rand(0, am_of_walls + am_of_tunnels) < am_of_walls) {
            if (abs(y_l - y_r) <= 1) {
                y_l = get_rand(1, h - 1);
            }
            if (abs(x_l - x_r) <= 1) {
                x_l = get_rand(1, w - 1);
            }
            render_line(vec2<int>(x_l, y_l), vec2<int>(y_r, x_r), arr, true);
        } else {
            render_line(vec2<int>(x_l, 0), vec2<int>(x_r, h - 1), arr, false);
            render_line(vec2<int>(0, y_l), vec2<int>(w - 1, y_r), arr, false);
        }
    }
    return arr;
}
