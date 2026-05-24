#include "cylinder.h"

#include "../util/vertex.h"

Cylinder::Cylinder(const Vec3f& color, int slices, bool useNormAsColor, bool drawNorms)
: drawNorms(drawNorms)
{
    auto [cylinderVerts, normVerts] = CreateCylinder(color, slices, useNormAsColor, drawNorms);
    topMesh    = Mesh(cylinderVerts[0]);
    bottomMesh = Mesh(cylinderVerts[1]);
    sideMesh   = Mesh(cylinderVerts[2]);
    normMesh   = Mesh(normVerts);
}

void Cylinder::draw(Mat4f modelMat, Mat4f viewMat, Mat4f projMat)
{
    topMesh.activate();
    glUniformMatrix4fv(0, 1, GL_FALSE, modelMat.Data());
    glUniformMatrix4fv(1, 1, GL_FALSE, viewMat.Data());
    glUniformMatrix4fv(2, 1, GL_FALSE, projMat.Data());

    topMesh.draw(GL_TRIANGLE_FAN);

    bottomMesh.activate();
    glUniformMatrix4fv(0, 1, GL_FALSE, modelMat.Data());
    glUniformMatrix4fv(1, 1, GL_FALSE, viewMat.Data());
    glUniformMatrix4fv(2, 1, GL_FALSE, projMat.Data());

    bottomMesh.draw(GL_TRIANGLE_FAN);

    sideMesh.activate();
    glUniformMatrix4fv(0, 1, GL_FALSE, modelMat.Data());
    glUniformMatrix4fv(1, 1, GL_FALSE, viewMat.Data());
    glUniformMatrix4fv(2, 1, GL_FALSE, projMat.Data());

    sideMesh.draw(GL_TRIANGLES);

    if(drawNorms) {
        normMesh.activate();
        glUniformMatrix4fv(0, 1, GL_FALSE, modelMat.Data());
        glUniformMatrix4fv(1, 1, GL_FALSE, viewMat.Data());
        glUniformMatrix4fv(2, 1, GL_FALSE, projMat.Data());

        normMesh.draw(GL_LINES);
    }
}

void Cylinder::deleteBuffers()
{
    topMesh.deleteBuffers();
    bottomMesh.deleteBuffers();
    sideMesh.deleteBuffers();
    normMesh.deleteBuffers();
}

std::tuple<std::array<std::vector<Vertex>,3>, std::vector<Vertex>>   CreateCylinder(const Vec3f& color, int slices, bool useNormAsColor, bool drawNorms)
{
    std::array<std::vector<Vertex>, 3> cylinderVerts;
    std::vector<Vertex> normVerts;

    const float r = 0.5f;
    const float h = 0.5f;

    CreateVertices(cylinderVerts[0],normVerts, Vec3f(0.f,  h, 0.f), Vec3f(0.f,  1.f, 0.f), color, useNormAsColor, drawNorms);
    CreateVertices(cylinderVerts[1],normVerts, Vec3f(0.f, -h, 0.f), Vec3f(0.f, -1.f, 0.f), color, useNormAsColor, drawNorms);
    
    for (int i = 0; i <= slices; i++)
    {
        float theta  = 2.0f * RAMEN_PI * float(i) / float(slices);
        float theta0 = 2.0f * RAMEN_PI * float(slices - i) / float(slices);
        float x  = r * sinf(theta);
        float z  = r * cosf(theta);
        float x0 = r * sinf(theta0);
        float z0 = r * cosf(theta0);

        CreateVertices(cylinderVerts[0],normVerts, Vec3f(x,   h, z),  Vec3f(0.f,  1.f, 0.f), color, useNormAsColor, drawNorms);
        CreateVertices(cylinderVerts[1],normVerts, Vec3f(x0, -h, z0), Vec3f(0.f, -1.f, 0.f), color, useNormAsColor, drawNorms);

    }

    for (int i = 0; i < slices; i++)
    {
        float theta     = 2.0f * RAMEN_PI * float(i) / float(slices);
        float thetaNext = 2.0f * RAMEN_PI * float(i + 1) / float(slices);

        float x  = r * sinf(theta);
        float z  = r * cosf(theta);
        float x2 = r * sinf(thetaNext);
        float z2 = r * cosf(thetaNext);

        Vec3f n1 = Vec3f(sinf(theta),     0.f, cosf(theta));
        Vec3f n2 = Vec3f(sinf(thetaNext), 0.f, cosf(thetaNext));

        Vec3f top1(x,   h, z);
        Vec3f bot1(x,  -h, z);
        Vec3f top2(x2,  h, z2);
        Vec3f bot2(x2, -h, z2);

        // Triangle 1
        CreateVertices(cylinderVerts[2],normVerts, top1,  n1, color, useNormAsColor, drawNorms);
        CreateVertices(cylinderVerts[2],normVerts, bot1,  n1, color, useNormAsColor, drawNorms);
        CreateVertices(cylinderVerts[2],normVerts, top2,  n2, color, useNormAsColor, drawNorms);


        // Triangle 2
        CreateVertices(cylinderVerts[2],normVerts, top2,  n2, color, useNormAsColor, drawNorms);
        CreateVertices(cylinderVerts[2],normVerts, bot1,  n1, color, useNormAsColor, drawNorms);
        CreateVertices(cylinderVerts[2],normVerts, bot2,  n2, color, useNormAsColor, drawNorms);
    }

    return {cylinderVerts, normVerts};
}
