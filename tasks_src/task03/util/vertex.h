#ifndef VERTEX_H
#define VERTEX_H

#include "../ramen/rgl_utils.h"

void CreateVertices(
    std::vector<Vertex>& objcetVerts,
    std::vector<Vertex>& normVerts,
    Vec3f position, Vec3f norm, Vec3f clr, 
    bool useNormAsColor = false, bool drawNorms = false);

#endif