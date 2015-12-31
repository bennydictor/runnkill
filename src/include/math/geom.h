#ifndef MATH_GEOM_H
#define MATH_GEOM_H


#include <math/vec3.h>
#include <math/ortohedron.h>

template <class T>
T Max(T a, T b) {
    if (a > b)
        return a;
    return b;
}

template <class T>
T dist(vec3<T> a, vec3<T> b) {
    return sqrt(Max((T)0, (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z)));
}

template <class T>
T dist(vec3<T> point, vec3<T> left, vec3<T> right) {
    for (int i = 0; i < 20; i++) {
        vec3<T> m1 = ((T)2 * left + right) / (T)3;
        vec3<T> m2 = (left + (T)2 * right) / (T)3;
        if (dist(m1, point) < dist(m2, point)) {
        
        }
    }
}

template <class T>
T dist_to_plain(vec3<T> plain, T d, vec3<T> point)
{
    return (point.dot(plain) + d) / dist(vec3<T>(0, 0, 0), plain);
}
template <class T>
vec3<T> plain(vec3<T> p1, vec3<T> p2, vec3<T> p3) {
    return vec3<T>(p1, p2).crs(vec3<T>(p1, p3));
}

template <class T>

T _intersect_segment_plain(vec3<T> plain1, vec3<T>plain2, vec3<T> plain3, vec3<T> seg1, vec3<T> seg2) {
    vec3<T> coeffs = plain(plain1, plain2, plain3);
    T d = -plain1.dot(coeffs);
    vec3<T> seg_par = vec3<T>(seg1, seg2);
    if (coeffs.dot(seg_par) == 0) {
        if ((coeffs.dot(seg1) + d)  == 0) {
            return 0;
        }
        return -1;
    }
    return -((coeffs.dot(seg1) + d) / coeffs.dot(seg_par));
}

template <class T>

bool intersect_seg_plain(vec3<T> plain1, vec3<T>plain2, vec3<T> plain3, vec3<T> seg1, vec3<T> seg2, vec3<T> &res) {
    T res1 = _intersect_segment_plain(plain1, plain2, plain3, seg1, seg2);
    if (res1 >= 0 and res1 < 1) {
        res = seg1 + res1 * vec3<T>(seg1, seg2);    
    }
//    std::cerr << res1 << std::std::endl;
    return (!(res1 < 0 or res1 > 1 + EPS_FOR_MOVE));
    
}

template <class T>

bool in_angle(vec3<T> plain1, vec3<T> plain2, vec3<T> plain3, vec3<T> point) {

    vec3<T> v1(plain1, plain2), v2(plain1, point), v3(plain1, plain3);
    vec3<T> crs1 = v1.crs(v2);
    vec3<T> crs2 = v1.crs(v3);
    vec3<T> crs3 = v2.crs(v3);
    return (crs1.dot(crs3) >= 0 and crs1.dot(crs2) >= 0);
}

template <class T>

bool intersect_seg_facet(vec3<T> plain1, vec3<T> plain2, vec3<T> plain3, vec3<T> seg1, vec3<T> seg2, vec3<T>& res) {
    vec3<T> plain4 = plain2 + vec3<T>(plain1, plain3);
    //std::cerr << ' ' << plain1 << ' ' << plain2 << ' ' << plain3 << ' ' << plain4 << '|' << seg1 << ' ' << seg2 << ';' << res << '\n';
    vec3<T> res1;
    if (!intersect_seg_plain(plain1, plain2, plain3, seg1, seg2, res1)) {
        return false;
    }
    bool ret = in_angle(plain1, plain2, plain3, res1) and in_angle(plain4, plain3, plain2, res1);
    if (ret and dist(seg1, res) > dist(seg1, res1))
        res = res1;
    return ret;
}


template <class T>

bool intersect_seg_ortohedron(ortohedron A, vec3<T> seg1, vec3<T> seg2, vec3<T>& res) {
    bool ret1 = intersect_seg_facet(A.p1, A.p2, A.p3, seg1, seg2, res) or
        intersect_seg_facet(A.p1, A.p2, A.p4, seg1, seg2, res) or
        intersect_seg_facet(A.p1, A.p3, A.p4, seg1, seg2, res);
    vec3<T> p5 = A.p2 + vec3<T>(A.p1, A.p3) + vec3<T>(A.p1, A.p4);
    bool ret2 = intersect_seg_facet(p5, A.p2 + vec3<T>(A.p1, A.p3), A.p3 + vec3<T>(A.p1, A.p4), seg1, seg2, res) or
        intersect_seg_facet(p5, A.p2 + vec3<T>(A.p1, A.p3), A.p4 + vec3<T>(A.p1, A.p2), seg1, seg2, res) or
        intersect_seg_facet(p5, A.p3 + vec3<T>(A.p1, A.p4), A.p4 + vec3<T>(A.p1, A.p2), seg1, seg2, res); 
    return ret1 || ret2;
}


template <class T>

bool intersect_point_sphere(vec3<T> point, vec3<T> centre, T rad, vec3<T> &res) {
    if (dist(point, centre) > rad)
        return false;
    res = vec3<T>(centre, point);
    res.resize(rad);
    res = res + centre;
    //std::cout << point << res << std::endl;
    return true;
}

template <class T>

bool intersect_segment_sphere(vec3<T> p1, vec3<T> p2, vec3<T> centre, T rad, vec3<T> &res, bool f=false) {
    vec3<T> ab(p1, p2), bc(centre, p1), ac(centre, p2);
    if (ab.sqlen() < EPS_FOR_MOVE)
        return intersect_point_sphere(p1, centre, rad, res);
    if ((ab.crs(bc).sqlen()) < EPS_FOR_MOVE) {
        if (vec3<T>(p1, centre).dot(vec3<T>(p2, centre)) > EPS_FOR_MOVE)
            return false;
        res = centre;
        //std::cout << "seg" << std::endl;
        return true;
    }
        
    T our_h = sqrt(bc.crs(ac).sqlen()) / (dist(p1, p2));
    vec3<T> our_plain = plain(p1, p2, centre), normal;
    normal = plain(p1, p2, p1 + our_plain);
    if (our_h > rad + EPS_FOR_MOVE) {
        return false;
    }
    normal.resize(our_h);
    vec3<T> projection = centre + normal;
    if ((ab.crs(vec3<T>(p1, projection)).sqlen()) > EPS_FOR_MOVE)
        projection = centre - normal;
    if (!f and vec3<T>(p1, projection).dot(vec3<T>(p2, projection)) > EPS_FOR_MOVE)
        return false;
    res = projection;
    //std::cout << "seg" << std::endl;
    return true;
}

template <class T>

bool intersect_plain_sphere(vec3<T> p1, vec3<T> p2, vec3<T> p3, vec3<T> centre, T rad, vec3<T> &res) {
    if (p1 == p2)
        return intersect_segment_sphere(p1, p3, centre, rad, res);
    if (p2 == p3)
        return intersect_segment_sphere(p1, p3, centre, rad, res);
    if (p1 == p3)
        return intersect_segment_sphere(p2, p3, centre, rad, res);
    vec3<T> cur_plain = plain(p1, p2, p3);
    T d = -cur_plain.dot(p1);
    if (fabs(dist_to_plain(cur_plain, d, centre)) > rad + EPS_FOR_MOVE)
        return false;
    vec3<T> normal = cur_plain;
    normal.resize(dist_to_plain(cur_plain, d, centre));
    if (dist_to_plain(cur_plain, d, centre + normal) * dist_to_plain(cur_plain, d, centre) <= 0) {
        res = centre + normal;
    } else {
        res = centre - normal;
    }
    return true;
}

template <class T>

bool intersect_facet_sphere(vec3<T> plain1, vec3<T> plain2, vec3<T> plain3, vec3<T> centre, T rad, vec3<T>& res) {
    vec3<T> plain4 = plain2 + vec3<T>(plain1, plain3);
    vec3<T> res1;
    if (intersect_plain_sphere(plain1, plain2, plain3, centre, rad, res1)) {
        if (in_angle(plain1, plain2, plain3, res1) and 
                    in_angle(plain4, plain3, plain2, res1)) {
            res = res1;
            //std::cout << "----------------------\n" << plain1 << plain2 << centre << rad << '\n' << plain3 << res << std::endl << "----------------------\n";
            return true;
        } 
    }
    return false;
}

template <class T>

bool intersect_ortohedron_sphere(ortohedron& d, vec3<T> centre, T rad, vec3<T>& res) {
    //Sorry for copypaste
    vec3<T> p5, p6, p7, p8;
    vec3<T> d1(d.p1, d.p2), d2(d.p1, d.p3), d3(d.p1, d.p4);
    p5 = d.p2 + d2;
    p6 = d.p2 + d3;
    p7 = d.p3 + d3;
    p8 = p7 + d1;
    //std::cout << "where" << centre << ' ' << rad << std::endl;
    //пересекаем с гранями
    if (intersect_facet_sphere(d.p1, d.p2, d.p3, centre, rad, res))
        return true;
    if (intersect_facet_sphere(d.p1, d.p2, d.p4, centre, rad, res))
        return true;
    if (intersect_facet_sphere(d.p1, d.p3, d.p4, centre, rad, res))
        return true;
    if (intersect_facet_sphere(p8, p5, p6, centre, rad, res))
        return true;
    if (intersect_facet_sphere(p8, p5, p7, centre, rad, res))
        return true;
    if (intersect_facet_sphere(p8, p6, p7, centre, rad, res))
        return true;
    //пересекаем с рёбрами
    if (intersect_segment_sphere(d.p1, d.p2, centre, rad, res))
        return true;
    if (intersect_segment_sphere(d.p1, d.p3, centre, rad, res))
        return true;
    if (intersect_segment_sphere(d.p1, d.p4, centre, rad, res))
        return true;
    if (intersect_segment_sphere(d.p2, p5, centre, rad, res))
        return true;
    if (intersect_segment_sphere(d.p2, p6, centre, rad, res))
        return true;
    if (intersect_segment_sphere(d.p3, p7, centre, rad, res))
        return true;
    if (intersect_segment_sphere(d.p3, p5, centre, rad, res))
        return true;
    if (intersect_segment_sphere(d.p4, p6, centre, rad, res))
        return true;
    if (intersect_segment_sphere(d.p4, p7, centre, rad, res))
        return true;
    if (intersect_segment_sphere(p5, p8, centre, rad, res))
        return true;
    if (intersect_segment_sphere(p6, p8, centre, rad, res))
        return true;
    if (intersect_segment_sphere(p7, p8, centre, rad, res))
        return true;
    //пересекаем с вершинами
    if (intersect_point_sphere(d.p1, centre, rad, res))
        return true;
    if (intersect_point_sphere(d.p2, centre, rad, res))
        return true;
    if (intersect_point_sphere(d.p3, centre, rad, res))
        return true;
    if (intersect_point_sphere(d.p4, centre, rad, res))
        return true;
    if (intersect_point_sphere(p5, centre, rad, res))
        return true;
    if (intersect_point_sphere(p6, centre, rad, res))
        return true;
    if (intersect_point_sphere(p7, centre, rad, res))
        return true;
    if (intersect_point_sphere(p8, centre, rad, res))
        return true;

    return false;
}

template <class T>

char intersect_segment_sphere_ortohedron(ortohedron& a, vec3<T> start, vec3<T> &finish, T rad, vec3<T> &res, int depth = 0)
{
    if (depth > 5) {
        return false;
    } 
    vec3<T> mid, low = start, hig = finish;
    if (intersect_ortohedron_sphere(a, low, rad, res)) {
        return -1;
    }
    if (!intersect_ortohedron_sphere(a, hig, rad, res)) {
        if (dist(start, finish) < SMALL_CONSTANT_1)
            return false;
        mid = (start + finish) / 2.0f;
        if (!intersect_segment_sphere_ortohedron(a, start, mid, rad, res, depth + 1))
            return intersect_segment_sphere_ortohedron(a, mid, finish, rad, res, depth + 1);
        finish = mid;
        return true;

    }
    for (int i = 0; i < 10; i++) {
        mid = (hig + low) / 2.0f;
        vec3<T> res1;
        if (intersect_ortohedron_sphere(a, mid, rad, res1)) {
            res = res1;
            hig = mid;
        } else {
            low = mid;
        }
    }
    finish = low;
    return true;
}
#endif // MATH_GEOM_H
