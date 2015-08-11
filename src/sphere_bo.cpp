#include <sphere_bo.h>
#include <math.h>


using namespace std;

vertex3d sphere_vbo_data[200];
short int sphere_ibo_data[200][4];

void gen_sphere() {
    int cnt = 0;
    for (double i = -M_PI; i < M_PI; i += M_PI / 10) {
        for (double j = 0; j < M_PI; j += M_PI / 10) {
            vertex3d curr;
            curr.normal[0] = curr.coord[0] = cosf(i) * sinf(j);
            curr.normal[1] = curr.coord[1] = cosf(j);
            curr.normal[2] = curr.coord[2] = sinf(i) * sinf(j);
            sphere_vbo_data[cnt] = curr;
            cnt++;
        }
    }
    cnt = 0;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            sphere_ibo_data[cnt][0] = i * 20 + j;
            if (j == 9) {
                sphere_ibo_data[cnt][1] = (i * 20 + j + 100) % 200;
                sphere_ibo_data[cnt][2] = (((1 + i) % 20) * 20 + j + 100) % 200;
                sphere_ibo_data[cnt][3] = (((1 + i) % 20) * 20 + j);
            } else {
                sphere_ibo_data[cnt][1] = (i * 20 + j + 1);
                sphere_ibo_data[cnt][2] = (((i + 1) % 20) * 20 + j + 1);
                sphere_ibo_data[cnt][3] = (((i + 1) % 20) * 20 + j);
            }
            cnt++;
        }
    }
}
