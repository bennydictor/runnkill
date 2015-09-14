#ifndef GRAPHICS_DRAW_OBJ_T_H
#define GRAPHICS_DRAW_OBJ_T_H

typedef struct {
    mat4f mat_m;
    void *ibo;
    unsigned int mode;
    unsigned int vbo;
    unsigned int count;
} draw_obj_t;

#endif // GRAPHICS_DRAW_OBJ_T_H
