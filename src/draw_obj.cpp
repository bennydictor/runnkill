#include <draw_obj.h>
#include <vector>
#include <vec3.h>

using namespace std;

vector<vector<vertex3d > > segments;
int sphere_ibo_data[8][HEIGHT * WIDTH][4];
draw_obj::draw_obj(int t, ortohedron point) {
    vector<vec3<float> > Points;
    points.resize(8);
    Points.push_back(point.p1);
    Points.push_back(point.p2);
    Points.push_back(point.p2 + vec3<float>(point.p1, point.p3));
    Points.push_back(point.p3);
    for (int i = 0; i < 4; i++)
    {
        Points.push_back(Points[i] + vec3<float>(point.p1, point.p4));
    }
    ibo_data = {0, 1, 2, 3, 0, 1, 5, 4, 0, 3, 7, 4, 6, 2, 1, 5, 6, 2, 3, 7, 6, 7, 4, 5};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                int pos = i * 4 + j * 2 + k;
                points[pos].coord[0] = Points[pos].x;
                points[pos].coord[1] = Points[pos].y;
                points[pos].coord[2] = Points[pos].z;

                points[pos].normal[0] = i * 2 - 1;
                points[pos].normal[1] = j * 2 - 1;
                points[pos].normal[2] = k * 2 - 1;
            }
        }
    }
}

draw_obj::draw_obj(int t, float rad, vec3<float> coords) {
    if (t == SPHERE) {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    for (int s = 0; s < (int)segments[i * 4 + j * 2 + k].size(); s++) {
                    
                        points.push_back(segments[i * 4 + j * 2 + k][s]);
                        for (int _ = 0; _ < 4; _++)
                            ibo_data.push_back(sphere_ibo_data[i * 4 + j * 2 + k][s][_]);
                    }
                }
            }
        }
    }
    for (int i = 0; i < points.size(); i++) {
        points[i].coord[0] = points[i].coord[0] * rad + coords.x;
        points[i].coord[1] = points[i].coord[1] * rad + coords.y;
        points[i].coord[2] = points[i].coord[2] * rad + coords.z;
    }
}

void fill_segments() {
    segments.resize(8);   
    for (int i = 0; i < HEIGHT1; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            float alpha = j * 2 * M_PI / WIDTH;
            float beta = i * M_PI / HEIGHT;
            vertex3d cur;
            cur.normal[0] = cur.coord[0] = sinf(beta) * cosf(alpha);
            cur.normal[1] = cur.coord[1] = cosf(beta);
            cur.normal[2] = cur.coord[2] = sinf(beta) * sinf(alpha);
            int dx, dy, dz;
            if (cur.coord[0] < 0) {
                dx = 1;
            }
            if (cur.coord[1] < 0) {
                dy = 1;
            }
            if (cur.coord[2] < 0) {
                dz = 1;
            }

            segments[dx * 4 + dy * 2 + dz].push_back(cur);
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            float alpha = j * 2 * M_PI / WIDTH;
            float beta = i * M_PI / HEIGHT;
            vertex3d cur;
            
            cur.normal[0] = cur.coord[0] = sinf(beta) * cosf(alpha);
            cur.normal[1] = cur.coord[1] = cosf(beta);
            cur.normal[2] = cur.coord[2] = sinf(beta) * sinf(alpha);
            int dx, dy, dz;
            if (cur.coord[0] < 0) {
                dx = 1;
            }
            if (cur.coord[1] < 0) {
                dy = 1;
            }
            if (cur.coord[2] < 0) {
                dz = 1;
            }
            int seg, pos;
            seg = dx * 4 + dy * 2 + dz;
            pos = i * j;
            sphere_ibo_data[seg][pos][0] = i * WIDTH + j;
            sphere_ibo_data[seg][pos][1] = (i + 1) * WIDTH + j;
            sphere_ibo_data[seg][pos][2] = (i + 1) * WIDTH + ((j + 1) % WIDTH);
            sphere_ibo_data[seg][pos][3] = i* WIDTH + ((j + 1) % WIDTH);
        }
    }
}
