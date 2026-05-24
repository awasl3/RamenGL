#include "mesh.h"

Mesh::Mesh() {}

Mesh::Mesh(const std::vector<Vertex>& verts)
{
    count = verts.size();
    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, count * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);

    glCreateVertexArrays(1, &vao);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
    /* Position */
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribBinding(vao, 0, 0);
    /* Normal */
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribBinding(vao, 1, 0);
    /* Color */
    glVertexArrayAttribFormat(vao, 2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float));
    glEnableVertexArrayAttrib(vao, 2);
    glVertexArrayAttribBinding(vao, 2, 0);
}

void Mesh::deleteBuffers()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Mesh::activate()
{
    glBindVertexArray(vao);
}

void Mesh::draw(int glForm)
{
    glDrawArrays(glForm, 0, count);
}
