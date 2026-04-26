#ifndef CUBE_H
#define CUBE_H

#include <vector>

#include "../ramen/rgl_utils.h"
#include "../../util/mesh.h"

std::tuple<std::vector<Vertex>, std::vector<Vertex>> CreateCuboid(const Vec3f& color, bool useNormAsColor = false, bool drawNorms = false);

struct Cube
{
    Mesh cubeMesh;
    Mesh normMesh;
    bool drawNorms;

    Cube(const Vec3f& color, bool useNormAsColor = false, bool drawNorms = false);

    void draw(Mat4f modelMat, Mat4f viewMat, Mat4f projMat);
    void deleteBuffers();
};

#endif