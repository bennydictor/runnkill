#include <field.h>
#include <cmath>

using namespace std;

template <class T>
void render_line(vec2<int> p1, vec2<int> p2, T**buf, T val) {
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

void render_line_add(vec2<int> p1, vec2<int> p2, int**buf, int val) {
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
        buf[steep ? y : x][steep ? x : y] += val;
        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
}
int get_rand(int l, int r) {
//    cout << l << ' ' << r <<  endl;
    return rand() % (r - l + 1) + l; 
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

    
int** gen_field_sun(int w, int h) {
    int **arr = new int*[w];
    for (int i = 0; i < w; i++) {
        arr[i] = new int[h];
    }
    for (int i = 0; i < w; i++) {
        arr[i][0] = 0;
        arr[i][h - 1] = 0;
    }
    for (int i = 0; i < h; i++) {
        arr[0][i] = 0;
        arr[w - 1][i] = 0;
    }
    for (int i = 1; i < w - 1; i++) {
        for (int j = 1; j < h - 1; j++) {
            arr[i][j] = 2;
        }
    }
    int lx, rx, ly, hy;
    
    vec2<int>** centres = new vec2<int>*[w / (BIG_WIGHT)];
    for (int i = 0; i < w / BIG_WIGHT; i++) {
        centres[i] = new vec2<int>[h / (BIG_WIGHT)];
    }
    for (int i = 0; i < w / (BIG_WIGHT); i++) {
        for (int j = 0; j < h / (BIG_WIGHT); j++) {
            lx = i * BIG_WIGHT;
            rx = (i + 1) * BIG_WIGHT;
            ly = j * BIG_WIGHT;
            hy = (j + 1) * BIG_WIGHT;
            int x = get_rand(lx + S_WIGHT / 2 + 1, rx - S_WIGHT / 2 - 2), y = get_rand(ly + S_WIGHT / 2 + 1, hy - S_WIGHT / 2 - 2);
            centres[i][j] = vec2<int>(x, y);
            for (int dx = -S_WIGHT / 2; dx < S_WIGHT / 2 + 1; dx++) {
                for (int dy = -S_WIGHT / 2; dy < S_WIGHT /2 + 1; dy++) {
                    arr[x + dx][y + dy] = 0;
                }
            } 
        }
    }

    int dx, dy;
    for (int i = 0; i < w / (BIG_WIGHT); i++) {
        for (int j = 0; j < h / (BIG_WIGHT); j++) {
            lx = get_rand(0, max(0, i - 1));
            rx = get_rand(min(w / BIG_WIGHT - 1, i + 1), w / (BIG_WIGHT) - 1);
            ly = get_rand(0, max(0, j - 1));
            hy = get_rand(min(h / BIG_WIGHT - 1, i + 1), h / (BIG_WIGHT) - 1);
            dx = get_rand(0, 2);
            dy = get_rand(0, 2);
            if (i == w / BIG_WIGHT - 1) {
                dx = 0;
                dy = 1;
            }
            if (dx == dy and dy == 0) {
                dx++;
                dy++;
            }
            render_line_add(centres[i][j], centres[min(w / BIG_WIGHT - 1, i + dx)][min(h / BIG_WIGHT - 1, j + dy)], arr, 10);
            /*
            if (get_rand(0, 1) == 0) {
                render_line_add(centres[i][j], centres[get_rand(0, w / BIG_WIGHT - 1)][hy], arr, 10);
            } else {
                render_line_add(centres[i][j], centres[rx][get_rand(0, h / BIG_WIGHT - 1)], arr, 10);
            }
            */
        }
    }
   
    for (int i = 0; i < w / (BIG_WIGHT); i++) {
        for (int j = 0; j < h / (BIG_WIGHT); j++) {
           arr[centres[i][j].x][centres[i][j].y] = 1; 
        }
    }
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            if (arr[i][j] > 40)
                arr[i][j] = 2;
            else if (arr[i][j] > 20)
                arr[i][j] = 1;
            else if (arr[i][j] > 2)
                arr[i][j] = 0;

        }
    }
    return arr;
}

int** gen_field_empty(int w, int h) {
    int** field;
    field = new int*[w];
    for (int i = 0; i < w; i++) {
        field[i] = new int[h];
        for (int j = 0; j < h; j++) {
            field[i][j] = 0;
        }
    }
    return field; 
}
