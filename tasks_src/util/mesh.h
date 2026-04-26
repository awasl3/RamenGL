#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <vector>

#include "../ramen/rgl_utils.h"

struct Mesh
{
    GLuint vbo, vao;
    int    count;

    Mesh();
    Mesh(const std::vector<Vertex>& verts);

    void deleteBuffers();
    void activate();
    void draw(int glForm = GL_TRIANGLES);
};

#endif