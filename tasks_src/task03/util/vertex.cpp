#include "vertex.h"

void CreateVertices(
    std::vector<Vertex>& objcetVerts,
    std::vector<Vertex>& normVerts, 
    Vec3f position, Vec3f norm, Vec3f clr, 
    bool useNormAsColor, bool drawNorms)
{
    Vec3f color = (useNormAsColor) ? norm : clr;
    objcetVerts.push_back(Vertex{ .position = position, .normal = norm, .color = color });
    if(drawNorms) {
        normVerts.push_back(Vertex{ .position = position, .normal = norm, .color = Vec3f{1.f, 1.f, 1.f}});
        Vec3f end =  position + norm;
        normVerts.push_back(Vertex{ .position = end, .normal = norm, .color = Vec3f{1.f, 1.f, 1.f}});
    }
}
