#include <game/armour.h>
#include <game/mod_type.h>

#include <graphics/draw_obj.h>
#include <graphics/material.h>
#include <graphics/vertex3d.h>
#include <graphics/gl.h>

#include <math/vec2.h>
#include <math/vecmath.h>
#include <math/geom.h>
#include <math/constants.h>
#define INF 10000000
#define _SQUARE(X) ((X) * (X))
using namespace std;

armour::armour() {

};
void armour::get(vec3<float> p1, vec3<float> p2, float angle, vector<vec3<float> >& vbo) {
    vector<vec3<float> > v_to_points(points.size());

    for (int i = 0; i < (int)v_to_points.size(); i++) {
        v_to_points[i] = vec3<float>(points[p1_idx], points[i]);
    }
    vec3<float> p1p2(p1, p2);
    vec2<float> xz(p1p2.x, p1p2.z), xz_was(v_to_points[p2_idx].x, v_to_points[p2_idx].z);
    float angle_xz = atan2(xz_was.crs(xz), xz_was.dot(xz));
//    cout << xz << ' ' << xz_was << ' ' << angle_xz << endl;
    for (int i = 0; i < (int)v_to_points.size(); i++) {
        if (i != p1_idx) {
            v_to_points[i].rotate(angle_xz, 0);
        } 
    }
    vec2<float> yz(p1p2.y, p1p2.z), yz_was(v_to_points[p2_idx].y, v_to_points[p2_idx].z);
    float angle_yz = atan2(yz_was.crs(yz), yz_was.dot(yz));
//    cout << yz << ' ' << yz_was << ' ' << angle_yz << endl;
    for (int i = 0; i < (int)v_to_points.size(); i++) {
        if (i != p1_idx) {
            v_to_points[i].rotate(0, 0, angle_yz);
        } 
    }
    vec2<float> xy(p1p2.x, p1p2.y), xy_was(v_to_points[p2_idx].x, v_to_points[p2_idx].y);
    float angle_xy = atan2(xy_was.crs(xy), xy_was.dot(xy));
    //cout << p1 << p2 << endl;
    for (int i = 0; i < (int)v_to_points.size(); i++) {
        if (i != p1_idx) {
            v_to_points[i].rotate(0, angle_xy);
            vec3<float> proj;
            vec3<float> curr_point = p1 + v_to_points[i];
            intersect_segment_sphere(p1, p2, curr_point, (float)INF, proj, true);
            vec3<float> curr_normal = plain(p1, p2, curr_point);
            vec3<float> curr_vec(proj, curr_point);
            if (curr_normal.sqlen() > EPS) { 
                curr_normal.resize(tan(angle) * sqrt(curr_vec.sqlen()));
                vec3<float> res = curr_vec + curr_normal;
                res.resize(sqrt(curr_vec.sqlen()));
                vbo.push_back(p1 + vec3<float>(p1, res + proj) / scale);
            } else {
                vbo.push_back(p1 + v_to_points[i] / scale);
            }
            
        } else {
            vbo.push_back(p1);
        }
    }
    //cout << v_to_points[p2_idx] << p1p2 << endl;
}


void armour::in(istream& stream) {
    int points_am;
    stream >> type;
    float r, g, b;
    stream >> r >> g >> b;
    _material.diffuse = new float[3]{r, g, b};
    stream >> r >> g >> b;
    _material.ambient = new float[3]{r, g, b};
    stream >> r >> g >> b;
    _material.specular = new float[3]{r, g, b};
    _material.shininess = 128;
    stream >> points_am;
    points.resize(points_am);
    normal.resize(points_am);
    for (int i = 0; i < points_am; i++) {
        stream >> points[i].x >> points[i].y >> points[i].z;

        normal[i].x = points[i].x;
        normal[i].y = points[i].y;
        normal[i].z = points[i].z;
        float len = sqrt(_SQUARE(normal[i].x) + _SQUARE(normal[i].y) + _SQUARE(normal[i].z)); 
        normal[i].x /= len;
        normal[i].y /= len;
        normal[i].z /= len;
    }
    int ibo_size;
    stream >> p1_idx >> p2_idx >> ibo_size;
    ibo_data = new unsigned int[4 * ibo_size];
    for (int i = 0; i < 4 * ibo_size; i++)
        stream >> ibo_data[i];
    stream >> scale;
    count = 4 * ibo_size;
    
    glGenBuffers(1, &vbo_buffer);
}


draw_obj armour::give_me_points(event E, event other) {
    vector<vec3<float> > vbo_to_get;
    vec3<float> p1p2(E.point1, E.point2);
    p1p2.rotate(other.angle);
    get(E.point1, E.point1 + p1p2, E.angle, vbo_to_get); 
    vertex3d *vbo_data = new vertex3d[vbo_to_get.size()];
    for (int i = 0; i < (int)vbo_to_get.size(); i++) {
        vbo_data[i].coord[0] = vbo_to_get[i].x + other.point1.x;// + coords.x;
        vbo_data[i].coord[1] = vbo_to_get[i].y + other.point1.y + 1;// + coords.y;
        vbo_data[i].coord[2] = vbo_to_get[i].z + other.point1.z;// + coords.z;
        vbo_data[i].normal[0] = normal[i].x;
        vbo_data[i].normal[1] = normal[i].y;
        vbo_data[i].normal[2] = normal[i].z;
        float len = sqrt(_SQUARE(vbo_data[i].normal[0]) + _SQUARE(vbo_data[i].normal[1]) + _SQUARE(vbo_data[i].normal[2])); 
        vbo_data[i].normal[0] /= len;
        vbo_data[i].normal[1] /= len;
        vbo_data[i].normal[2] /= len;
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo_buffer);
    glBufferData(GL_ARRAY_BUFFER, vbo_to_get.size() * sizeof(vertex3d), vbo_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    draw_obj ret;
    ret.mat_m = make_mat4();
    id_mat4(ret.mat_m);
    ret.mode = GL_QUADS;
    ret.vbo = vbo_buffer;
    ret.ibo = ibo_data;
    ret.count = count;
    ret.material = _material;
    ret.free_mat_m = 0;
    ret.free_ibo = 0;
    return ret;
}
