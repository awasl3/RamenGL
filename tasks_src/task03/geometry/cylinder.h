#ifndef CYLINDER_H
#define CYLINDER_H

#include <array>
#include <vector>

#include "../ramen/rgl_utils.h"
#include "../../util/mesh.h"

std::tuple<std::array<std::vector<Vertex>,3>, std::vector<Vertex>> CreateCylinder(const Vec3f& color, int slices = 32, bool useNormAsColor = false, bool drawNorms = false);

struct Cylinder
{
    Mesh topMesh;
    Mesh bottomMesh;
    Mesh sideMesh;
    Mesh normMesh;
    bool drawNorms;

    Cylinder(const Vec3f& color, int slices = 32, bool useNormAsColor = false, bool drawNorms = false);

    void draw(Mat4f modelMat, Mat4f viewMat, Mat4f projMat);
    void deleteBuffers();
};

#endif