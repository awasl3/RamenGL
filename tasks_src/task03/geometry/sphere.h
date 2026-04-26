#ifndef SPHERE_H
#define SPHERE_H

#include <vector>

#include "../ramen/rgl_utils.h"
#include "../../util/mesh.h"

std::tuple<std::vector<Vertex>, std::vector<Vertex>>  CreateSphere(const Vec3f& color, int tesselation = 32, bool useNormAsColor = false, bool drawNorms = false);

struct Sphere
{
    Mesh sphereMesh;
    Mesh normMesh;
    bool drawNorms;

    Sphere(const Vec3f& color, int tesselation = 32, bool useNormAsColor = false, bool drawNorms = false);

    void draw(Mat4f modelMat, Mat4f viewMat, Mat4f projMat);
    void deleteBuffers();
};

#endif